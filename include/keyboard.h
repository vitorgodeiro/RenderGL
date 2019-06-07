void inputKeyboard(unsigned char key, int _x, int _y){
	std::system("clear");
	std::cin.clear();
	fflush(stdin);
	switch (key){
		case 'a' :
		case 'A' :
			if (typeFrontFace == GL_CCW){
				typeFrontFace = GL_CW;
				glFrontFace(typeFrontFace);
				ccw = true;
				std::cout << "Activate Clock-Wise" << std::endl;
			}
			else{
				typeFrontFace = GL_CCW;
				glFrontFace(typeFrontFace);
				ccw = false;
				std::cout << "Activate Counter Clock-Wise" << std::endl;
			}
			updateMVP();
		break;
		case 'b' :
		case 'B' :
			std::cout << "The actual size is: (" << width << ", " << height << ")" << std::endl;
			std::cout << "Enter the new value of Width:" << std::endl;
			std::cin >> width;
			std::cout << "Enter the new value of height:" << std::endl;
			std::cin >> height;
			std::cout << "The actual size is: (" << width << ", " << height << ")" << std::endl;
			glutReshapeWindow(width, height);
			updateMVP();
			display();
			break;
		case 'c' :
		case 'C' : {
			std::cout << "The actual colors in RGB is:" << std::endl;
			std::cout << "R = " << color[0] << " G = " << color[1] << " B = " << color[2] << std::endl;
			std::cout << "Enter the new value of R:" << std::endl;
			std::cin >> color[0];
			std::cout << "Enter the new values of G:" << std::endl;
			std::cin >> color[1];
			std::cout << "Enter the new values of B" << std::endl;
			std::cin >> color[2];
			std::cout << "The actual colors in RGB is:" << std::endl;
			std::cout << "R = " << color[0] << " G = " << color[1] << " B = " << color[2] << std::endl;
			glUniform3fv(uniColor, 1, color);
			updateMVP();
			break;}
		case 'e' :
		case 'E' :
			if (!closeGL) {	glEnable(GL_CULL_FACE);  glCullFace(GL_BACK); }
			std::cout << "Enable Back-Face Culling" << std::endl;
			backFaceGL = true;
			updateMVP();
			break;
		case 'd' :
		case 'D' :
			glDisable(GL_CULL_FACE);  
			std::cout << "Disable Back-Face Culling" << std::endl;
			backFaceGL = false;
			updateMVP();
			break;
		case 'f' :
		case 'F' : {
			std::cout << "The actual valor of Far Clipping Planes: " <<  zFar << std::endl;
			std::cout << "Enter the new value of Far Clipping Planes:" << std::endl;
			std::cin >> zFar;
			std::cout << "The actual valor of Far Clipping Planes: " <<  zFar << std::endl;			
			updateMVP();
			break;}
		case 'n' :
		case 'N' : {
			std::cout << "The actual valor of Near Clipping Planes: " <<  zNear << std::endl;
			std::cout << "Enter the new value of Near Clipping Planes:" << std::endl;
			std::cin >> zNear;
			std::cout << "The actual valor of Near Clipping Planes: " <<  zNear << std::endl;			
			updateMVP();
			break;}
		case 'm':
		case 'M':{
			textureType++;
			textureType = textureType % 3;
			updateMVP();
			break;
		}
		case 'p' :
		case 'P' : 
			//type = GL_POINTS;
			typeFormRender = 2;
			updateMVP();
			break;
		case 'q' :
		case 'Q' : 
			exit(0);
			break;
		case 'r' :
		case 'R' : {
			float *max = mesh->box.getMax();
			float *min = mesh->box.getMin();
 			r = std::sqrt(std::pow((max[0]-min[0]), 2) + std::pow((max[1]-min[1]), 2)+std::pow((max[2]-min[2]), 2));
			fDistance =  r/tan( 30 * PI / 180.0f );
			zNear = fDistance - r;
			zFar = fDistance + r;
			eye = glm::vec3(0.0f, 0.0f, fDistance);
			lookDir = glm::vec3(0.0f, 0.0f, -1.0f);
			up = glm::vec3(0,1,0);
			right = glm::vec3(1.0f ,0.0f, 0.0f);
			updateMVP();
			break;}	
				
		case 't' :
		case 'T' : 
			typeFormRender = 0;
			//glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
			type = GL_TRIANGLES;
			updateMVP();
			break;
		case 'w' :
		case 'W' : 
			typeFormRender = 1;
			//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
			type = GL_TRIANGLES;
			updateMVP();
			break;
/////// Roll (z)
		case 'u' :
		case 'U' : {
			glm::vec3 u = right, v = up;
			right = u*COSTHETA + v*SINTHETA;
			up = -u*SINTHETA + v*COSTHETA;
			updateMVP();
			break;}
		case 'j' :
		case 'J' : {
			glm::vec3 u = right, v = up;
			right = u*COSTHETA + -v*SINTHETA;
			up = u*SINTHETA + v*COSTHETA;
			updateMVP();
			break;}
/////////// Yaw (y)
		case 'h' :
		case 'H' : {
			glm::vec3 u = right, n = lookDir;
      		right = u*COSTHETA + n*SINTHETA;
			lookDir = -u*SINTHETA + n*COSTHETA;
			updateMVP();
			break;
			}
		case 'k' :
		case 'K' : {
			glm::vec3 u = right, n = lookDir;
      		right = u*COSTHETA + -n*SINTHETA;
			lookDir = u*SINTHETA + n*COSTHETA;
			updateMVP();
			break;
			}
/////////// Pitch (x)
		case 'l' :
		case 'L' :{
			glm::vec3 v = up, n = lookDir;
      		up = v*COSTHETA + n*SINTHETA;
			lookDir = -v*SINTHETA + n*COSTHETA;
			updateMVP();
			break;
			}
		case 'o' :
		case 'O' :{
			glm::vec3 v = up, n = lookDir;
      		up = v*COSTHETA + -n*SINTHETA;
			lookDir = v*SINTHETA + n*COSTHETA;
			updateMVP();
			break;
			}
		case 'z':
		case 'Z':{
			closeGL = !closeGL;
			std:: cout << "Change to  ";
			if (closeGL) {std::cout << "CloseGL";glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &shadingNormal);
				glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &shadingNormalF);} else {std::cout << "OpenGL";}
			std::cout << std::endl;
			updateMVP();
			break;
		}
//
		case 'v' :{
			checkShacder++;
			if(checkShacder == 0){
				glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &shadingNormal);
				glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &shadingNormalF);
				std::cout << "Normal shading" << std::endl;
				phongGL = 0;
				updateMVP();
			}else if (checkShacder == 1){
				glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &shadingGoroud);
				glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &shadingNormalF);
				std::cout << "Goroud shading" << std::endl;
				phongGL = 1;
				updateMVP();
			}else if (checkShacder == 2){
				glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &shadingGoroudFull);
				glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &shadingNormalF);
				std::cout << "Goroud Full shading" << std::endl;
				phongGL = 1;
				updateMVP();
			}else if(checkShacder == 3){				
				glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &shadingPhong);
				glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &shadingPhongF);
				std::cout << "Phong shading" << std::endl;
				checkShacder = -1;
				phongGL = 2;
				updateMVP();
			}
			
			
			updateMVP();
			break;
		}
		case '1' :
			std:: cout << "Translate Pitch Center : " << -step << std::endl;	
			eye += lookDir*step;
			updateMVP();
			break;	
		case '2' :
			std:: cout << "Translate Yaw Center : " << -step << std::endl;	
			eye -= up*step;
			lookDir = glm::normalize(-eye);
			up = glm::normalize(glm::cross(right, lookDir));
			updateMVP();
			break;	
		case '3' :
			std:: cout << "Translate Pitch Center : " << step << std::endl;	
			eye -= lookDir*step;
			updateMVP();
			break;	
		case '4' :{
			std:: cout << "Translate Roll Center : " << -step << std::endl;
			eye -= right*step;
			lookDir = glm::normalize(-eye);
			right = glm::normalize(glm::cross(lookDir, up));
			updateMVP();
			break;}
		case '6' :
			std:: cout << "Translate Roll Center : " << step << std::endl;
			eye += right*step;
			lookDir = glm::normalize(-eye);
			right = glm::normalize(glm::cross(lookDir, up));
			updateMVP();
			break;
		case '8' :
			std:: cout << "Translate Yaw Center : " << step << std::endl;	
			eye += up*step;
			lookDir = glm::normalize(-eye);
			up = glm::normalize(glm::cross(right, lookDir));
			updateMVP();
			break;
		case ';' :
			std:: cout << "Translate Yaw : " << -step << std::endl;	
			eye -= up*step;
			updateMVP();
			break;	
		case ',' :{
			std:: cout << "Translate Roll : " << -step << std::endl;
			eye -= right*step;
			updateMVP();
			break;}
		case '.' :
			std:: cout << "Translate Roll : " << step << std::endl;
			eye += right*step;
			updateMVP();
			break;
		case '/' :
			std:: cout << "Translate Yaw : " << step << std::endl;	
			eye += up*step;
			updateMVP();
			break;
			
	}

	glutPostRedisplay();
}