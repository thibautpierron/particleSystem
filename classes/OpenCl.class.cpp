#include "OpenCl.class.hpp"

OpenCl::OpenCl() {
    return;
}

OpenCl::OpenCl(std::string kernelSrc)
         : error(0), platform(NULL), numPlatform(0), context(0) {
    
    error = clGetPlatformIDs(1, &platform, &numPlatform);
    checkError(error, "Error finding platform ids.");

    error = clGetPlatformIDs(numPlatform, &platform, NULL);
    checkError(error, "Error finding platform.");

    error = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 0, NULL, &deviceNbr);
    checkError(error, "Error finding devices number.");

    devicesID = new cl_device_id[deviceNbr];
    error = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, deviceNbr, devicesID, NULL);
    checkError(error, "Error finding devices id.");

    getInteropCompatibleDevice();

    CGLContextObj CGLContext = CGLGetCurrentContext();
    CGLShareGroupObj CGLShareGroup = CGLGetShareGroup(CGLContext);
    cl_context_properties props[] = { CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE,
                                    (cl_context_properties)CGLShareGroup,
                                    0 };
    context = clCreateContext(props, 1, &device, NULL, NULL, &error);
    checkError(error, "Error creating context.");
    
    queue = clCreateCommandQueue(context, device, 0, &error);
    checkError(error, "Error creating command queue.");

    loadKernel(kernelSrc);
    extractKernel();

    return;
}

OpenCl::~OpenCl() {
    delete devicesID;
    return;
}

void OpenCl::loadKernel(std::string path) {
    std::ifstream stream(path);
    std::string buff;
    std::string src;

    if(!stream) {
        std::cout << "Shader File: " << path << " not found." << std::endl;
        exit(0);
    }

    while(getline(stream, buff))
        src += buff + '\n';
    src += '\n';
    stream.close();

    this->kernelSrcSize = src.size();
    this->kernelSrc = new char [this->kernelSrcSize];
    std::strcpy(this->kernelSrc, src.c_str());

    return;
}

void     OpenCl::getInteropCompatibleDevice() {
    unsigned int i = 0;
    size_t sizeRet;
    while(i < deviceNbr) {
        error = clGetDeviceInfo(devicesID[i], CL_DEVICE_EXTENSIONS, 0, NULL, &sizeRet);
        checkError(error, "Error finding extension size.");
        if(sizeRet > 0) {
            char *extension = (char*)malloc(sizeRet);
            error = clGetDeviceInfo(devicesID[i], CL_DEVICE_EXTENSIONS, sizeRet, extension, &sizeRet);
            checkError(error, "Error finding extension infos.");
            std::string str(extension);
            free(extension);
            if(str.find("cl_APPLE_gl_sharing")!= std::string::npos) {
                this->device = devicesID[i];
                return;
            }
        }
        i++;
    }
    std::cout << "No compatible device found." << std::endl;
    exit(0);
    return;
}

void    OpenCl::extractKernel() {
    program = clCreateProgramWithSource(context, 1, (const char**)&kernelSrc, &kernelSrcSize, &error);
    checkError(error, "Error creating program.");

    error = clBuildProgram(program, 1, &device, "-cl-fast-relaxed-math", NULL, NULL);
    if(error != CL_SUCCESS) {
        char buildLog[10240];
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, sizeof(buildLog), buildLog, NULL);
        std::cout << buildLog << std::endl;
        checkError(error, "Error building program.");
    }

    gravityKernel = clCreateKernel(program, "gravity", &error);
    initKernels.push_back(clCreateKernel(program, "init_cube", &error));
    initKernels.push_back(clCreateKernel(program, "init_sphere", &error));
    initKernels.push_back(clCreateKernel(program, "init_line", &error));
    return;
}

void     OpenCl::checkError(cl_int error, std::string msg) {
    std::string errorStr;

    if(error != CL_SUCCESS) {
        switch(error) {
            case -11: errorStr = " CL_BUILD_PROGRAM_FAILURE"; break;
            case -30: errorStr = " CL_INVALID_VALUE"; break;
            default: errorStr = std::to_string(error); break;
        }
        std::cout << msg << errorStr << std::endl;
        exit(0);
    }
}

void OpenCl::createSharedVBO(GLuint buf) {
    glFinish();
    sharedVBO = clCreateFromGLBuffer(context, CL_MEM_READ_WRITE, buf, &error);
    checkError(error, "Error creating sharedVBO.");

    error = clSetKernelArg(gravityKernel, 0, sizeof(cl_mem), &sharedVBO);
    checkError(error, "Error setting kernel argument.");

    for (unsigned int i = 0; i < initKernels.size(); i++) {
        error = clSetKernelArg(initKernels[i], 0, sizeof(cl_mem), &sharedVBO);
        checkError(error, "Error setting init kernel[" + std::to_string(i) + "] argument.");
    }
}

void OpenCl::runGravityKernel(Vec4 mousePosition) {

    glFinish();
    error = clSetKernelArg(gravityKernel, 2, sizeof(Vec4), &mousePosition);

    error = clEnqueueAcquireGLObjects(queue, 1, &sharedVBO, 0, 0, 0);
    checkError(error, "AcquireGLObjects failed");

    error = clEnqueueNDRangeKernel(queue, gravityKernel, 1, NULL, &bufferItemNumber, NULL, 0,0,0);
    checkError(error, "Run gravity kernel failed");
    error = clEnqueueReleaseGLObjects(queue, 1, &sharedVBO, 0, 0, 0);
    checkError(error, "ReleaseGLObjects failed");

    clFinish(queue);
    return;
}

void OpenCl::runInitKernel(unsigned int n) {
    if (n >= initKernels.size())
        return;
    error = clEnqueueAcquireGLObjects(queue, 1, &sharedVBO, 0, 0, 0);
    checkError(error, "AcquireGLObjects failed");
    error = clEnqueueNDRangeKernel(queue, initKernels[n], 1, NULL, &bufferItemNumber, NULL, 0,0,0);
    checkError(error, "Run init kernel[" + std::to_string(n) + "] failed");
    error = clEnqueueReleaseGLObjects(queue, 1, &sharedVBO, 0, 0, 0);
    checkError(error, "ReleaseGLObjects failed");
    return;
}

void OpenCl::createOnlyClBuffer(unsigned int arrayLength) {

    this->bufferItemNumber = arrayLength;

    velocityVBO = clCreateBuffer(context, CL_MEM_READ_WRITE, arrayLength * sizeof(Vec4), NULL, &error);
    checkError(error, "Error creating clbuffer.");

    error = clSetKernelArg(gravityKernel, 1, sizeof(cl_mem), &velocityVBO);
    checkError(error, "Error setting kernel argument.");

    for (unsigned int i = 0; i < initKernels.size(); i++) {
        error = clSetKernelArg(initKernels[i], 1, sizeof(cl_mem), &velocityVBO);
        checkError(error, "Error setting init kernel[" + std::to_string(i) + "] argument.");
    }
}