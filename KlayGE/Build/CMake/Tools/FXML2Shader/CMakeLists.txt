SET(SOURCE_FILES
	${KLAYGE_PROJECT_DIR}/Tools/src/FXML2Shader/FXML2Shader.cpp
)

SETUP_TOOL(FXML2Shader)

target_link_libraries(FXML2Shader
	PRIVATE
		${KLAYGE_FILESYSTEM_LIBRARY}
)
