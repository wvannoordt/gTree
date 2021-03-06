#include "ICmfMeshArrayHandler.h"
#include "ICmfMesh.h"
#include "StringUtils.h"
#include "CmfDataBase.h"
namespace cmf
{
    ICmfMeshArrayHandler::ICmfMeshArrayHandler(void)
    {
        
    }
    
    ICmfMeshArrayHandler::~ICmfMeshArrayHandler(void)
    {
        Destroy();
    }
    
    ICmfMeshArray* ICmfMeshArrayHandler::CreateNewVariable(ArrayInfo info)
    {
        return NULL;
    }
    
    bool ICmfMeshArrayHandler::VariableExists(std::string name)
    {
        return (varList.find(name)!=varList.end());
    }
    
    ParallelGroup* ICmfMeshArrayHandler::GetDatabaseParallelGroup(void)
    {
        return this->Mesh()->GetGroup();
    }
    
    ICmfMesh* ICmfMeshArrayHandler::Mesh(void)
    {
        return baseMesh;
    }
    
    void ICmfMeshArrayHandler::Destroy(void)
    {
        for (std::map<std::string, ICmfMeshArray*>::iterator it = varList.begin(); it != varList.end(); it++)
        {
            WriteLine(6, "Free var \"" + it->first + "\"");
            it->second->Destroy();
            delete it->second;
        }
    }
}