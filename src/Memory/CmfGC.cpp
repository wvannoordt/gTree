#include "CmfGC.h"
#include "CmfScreen.h"
#include "StringUtils.h"
#include <iostream>
#include "DebugTools.hx"
#include "ParallelGroup.h"
#include "cmf.h"
#include "CmfCuda.h"
#include "CmfError.h"

namespace cmf
{
    CmfGC cmfGC;
    CmfGC::CmfGC(void)
    {
        allocSize = 0;
        totalAlloc=0;
        totalFree=0;
        stackBlobSize=CMF_STACK_BLOB_SIZE;
        stackBlobIdx = 0;
        allocSizeGPU = 0;
        totalAllocGPU = 0;
        totalFreeGPU = 0;
    }
    
    CmfGC::~CmfGC(void)
    {
        WriteLineStd(3, "Freeing resources");
        WriteLineStd(3, "Total allocs: " + NiceCommaString(totalAlloc));
        WriteLineStd(3, "Total frees: " + NiceCommaString(totalFree));
    }
    
    void* CmfGC::CmfGpuAlloc(size_t size, const int deviceNum, const char* file, const int line)
    {
        totalAllocGPU++;
        allocSizeGPU += size;
        if (!CUDA_ENABLE) CmfError("Cmf_GpuAlloc has been called, but CMF was compiled without CUDA support");
        void* pointerOut;
#if(CUDA_ENABLE)
        CMF_CUDA_CHECK(cudaSetDevice(deviceNum));
        CMF_CUDA_CHECK(cudaMalloc(&pointerOut, size));
        WriteLine(5, "Allocating GPU buffer, cumulative size: " + NiceCommaString(allocSizeGPU));
#endif
        return pointerOut;
    }
    
    void CmfGC::CmfGpuFree(void* ptr, const char* file, const int line, const char* varname)
    {
        if (!CUDA_ENABLE) CmfError("Cmf_GpuFree has been called, but CMF was compiled without CUDA support");
    #if(CUDA_ENABLE)
        CMF_CUDA_CHECK(cudaFree(ptr));
        std::string filestr(file);
        std::string varstr(varname);
        WriteLine(5, "Freeing GPU variable " + varstr);
        WriteLine(6, "From file " + filestr + ", line " + std::to_string(line));
    #endif    
        totalFreeGPU++;
    }
    
    void* CmfGC::CmfAlloc(size_t size, const char* file, const int line, bool allowStackAllocation)
    {
        bool stackAlloc = false;
#if(CMF_ENABLE_STACK_BLOB)
        stackAlloc = (stackBlobIdx+size < CMF_STACK_BLOB_SIZE) && allowStackAllocation && globalSettings.allowStackAllocation;
#endif
        totalAlloc++;
        allocSize += size;
        if (!stackAlloc) {WriteLine(5, "Allocating buffer, cumulative size: " + NiceCommaString(allocSize));}
        else {WriteLine(5, "Allocating (stack) buffer, cumulative size: " + NiceCommaString(allocSize));}
        std::string filestr(file);
        WriteLine(6, "From file " + filestr + ", line " + std::to_string(line));
        if (stackAlloc)
        {
            stackAllocatedPointers.insert({(void*)(stackBlob+stackBlobIdx),size});
            void* output = stackBlob+stackBlobIdx;
            stackBlobIdx += size;
            return output;
        }
        void* output = malloc(size);
        return output;
    }
    
    void CmfGC::CmfFree(void* ptr, const char* file, const int line, const char* varname, bool allowStackAllocation)
    {
        bool stackFree = false;
#if(CMF_ENABLE_STACK_BLOB)
        stackFree = (stackAllocatedPointers.find((char*)ptr) != stackAllocatedPointers.end()) && allowStackAllocation && globalSettings.allowStackAllocation;
#endif
        totalFree++;
        std::string filestr(file);
        std::string varstr(varname);
        if (!stackFree) {WriteLine(5, "Freeing variable " + varstr);}
        else {WriteLine(5, "Freeing (stack) variable " + varstr);}
        WriteLine(6, "From file " + filestr + ", line " + std::to_string(line));
        if (stackFree) {return;}
        free(ptr);
    }
}