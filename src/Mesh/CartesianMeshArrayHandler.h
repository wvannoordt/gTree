#ifndef CMF_CARTESIAN_MESH_ARRAY_HANDLER
#define CMF_CARTESIAN_MESH_ARRAY_HANDLER
#include "ICmfMeshArrayHandler.h"
#include "CartesianMeshArray.h"
#include "AmrFcnTypes.h"
#include <string>
#include <map>
namespace cmf
{
    class CartesianMesh;
    /// @brief Defines a MeshArrayHandler object for Cartesian meshes
    /// @author WVN
    class CartesianMeshArrayHandler : public ICmfMeshArrayHandler
    {
        friend class CartesianMeshArray;
        public:
            /// @brief Constructor
            /// @author WVN
            CartesianMeshArrayHandler(CartesianMesh* mesh_in);
            
            /// @brief Destructor
            /// @author WVN
            ~CartesianMeshArrayHandler(void);
            
            /// @brief Creates a new variable with the given name
            /// @param info Information about the variable \see ArrayInfo
            /// @param filter A block filter defining the domain of the variable
            /// @author WVN
            CartesianMeshArray* CreateNewVariable(ArrayInfo info, NodeFilter_t filter);
            
            /// @brief Checks if a variable with the given name exists
            /// @param name The name to check
            /// @author WVN
            bool VariableExists(std::string name);
        
        private:
            
            /// @brief The underlying mesh object
            CartesianMesh* mesh;
            
    };
}

#endif