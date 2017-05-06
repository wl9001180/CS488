#pragma once

#include "cs488-framework/CS488Window.hpp"
#include "cs488-framework/OpenGLImport.hpp"
#include "cs488-framework/ShaderProgram.hpp"
#include "cs488-framework/MeshConsolidator.hpp"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include "mesh_obj.hpp"
#include "SceneNode.hpp"

#include <glm/glm.hpp>
#include <memory>

#define  DIR_NONE      0x00 
#define  DIR_X         0x01     
#define  DIR_Y         0x02    
#define  DIR_Z         0x04   

#define  OBJ_CUBE      1
#define  OBJ_PLANE     2

#define  SENS_PANX     30.0
#define  SENS_PANY     23.0
#define  SENS_ZOOM     35.0
struct LightSource {
	glm::vec3 position;
	glm::vec3 rgbIntensity;
};

typedef GLfloat Matrix[4][4];

extern Matrix mIdentity;
class A3 : public CS488Window {
public:
	A3(const std::string & luaSceneFile);
	virtual ~A3();



protected:





	virtual void init() override;
	virtual void appLogic() override;
	virtual void guiLogic() override;
	virtual void draw() override;
	virtual void cleanup() override;

	//-- Virtual callback methods
	

	virtual bool cursorEnterWindowEvent(int entered) override;
	virtual bool mouseMoveEvent(double xPos, double yPos) override;
	virtual bool mouseButtonInputEvent(int button, int actions, int mods) override;
	virtual bool mouseScrollEvent(double xOffSet, double yOffSet) override;
	virtual bool windowResizeEvent(int width, int height) override;
	virtual bool keyInputEvent(int key, int action, int mods) override;
	virtual void vPerformTransfo(float fOldX, float fNewX, float fOldY, float fNewY);
	virtual void vCalcRotVec(float fNewX, float fNewY,
                 float fOldX, float fOldY,
                 float fDiameter,
                 float *fVecX, float *fVecY, float *fVecZ); 
	virtual void vAxisRotMatrix(float fVecX, float fVecY, float fVecZ, Matrix mNewMat) ;
	//-- One time initialization methods:
	void processLuaSceneFile(const std::string & filename);
	void createShaderProgram();
	void enableVertexShaderInputSlots();
	void uploadVertexDataToVbos();
	void mapVboDataToVertexShaderInputLocations();
	void initViewMatrix();
	void initLightSources();
    void find_parents(int id, SceneNode * root, SceneNode * root_org, float rot_x, float rot_y);
    void find_head(SceneNode * root, SceneNode * root_org, float rot_x, float rot_y);

	void initPerspectiveMatrix();
	void uploadCommonSceneUniforms();
	void renderSceneGraph(const SceneNode &node);
	void renderSceneGraph_helper(glm::mat4 trans, const SceneNode * node);
	void renderArcCircle();
	void save_current(SceneNode *root);
	void load(SceneNode *root, int a);
	void reset_joint(SceneNode *root);
	glm::mat4 m_perpsective;
	glm::mat4 m_view;

	LightSource m_light;

	//-- GL resources for mesh geometry data:

	//board
	GLuint new_vao;	
	GLuint new_vbo_vertexPositions;
	GLuint new_vbo_vertexNormals;
	GLuint new_vbo_UV;
	GLint new_positionAttribLocation;	
	GLint new_normalAttribLocation;
	GLint new_UVLocation;

	//pieces
	GLuint piece_vao;	
	GLuint piece_vbo_vertexPositions;
	GLuint piece_vbo_vertexNormals;
	GLuint piece_vbo_UV;
	GLint piece_positionAttribLocation;	
	GLint piece_normalAttribLocation;
	GLint piece_UVLocation;


	ShaderProgram m_shader;
	ShaderProgram new_shader;

	//-- GL resources for trackball circle geometry:
	GLuint m_vbo_arcCircle;
	GLuint m_vao_arcCircle;
	GLint m_arc_positionAttribLocation;
	ShaderProgram m_shader_arcCircle;

	// BatchInfoMap is an associative container that maps a unique MeshId to a BatchInfo
	// object. Each BatchInfo object contains an index offset and the number of indices
	// required to render the mesh with identifier MeshId.
	BatchInfoMap m_batchInfoMap;


	//for picking
	GLuint color_renderbuffer;
	GLuint depth_renderbuffer;
	GLuint framebuffer;

	

	std::string m_luaSceneFile;

	std::shared_ptr<SceneNode> m_rootNode;
};