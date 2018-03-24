#include "Utilities.h"
#include "Constantes.h"
#include "Logger.h"

int Utilities::ID_INCREMENTAL = 1;

int Utilities::getNewID(){
	return Utilities::ID_INCREMENTAL++;
}

int Utilities::ALTO_PX = -1; 
int Utilities::ANCHO_PX = -1; 
int Utilities::ALTO_UL = -1; 
int Utilities::ANCHO_UL = -1; 
int Utilities::ID_SNOWBOY = -1;

bool Utilities::PAUSA_CLIENTE = false;

std::string Utilities::IMAGEN_FONDO_NIVEL1 = "";

int Utilities::sdl2euclid_ejeY(int y){
	return Utilities::ALTO_PX - y;
}
int Utilities::euclid2sdl_ejeY(int y){
	return Utilities::ALTO_PX - y;
}

// strColorHexa es de la forma: "#FFAA00"
void Utilities::strColorHexaToRGB(std::string strColorHexa, int* vectorRGB){
	strColorHexa = strColorHexa.substr(1,6);
	const char* HexColorPtr = strColorHexa.c_str();
	
	for (int i = 0; i < 3; i++){
		int IntColor = (Utilities::GetDecimal(HexColorPtr[0]) * 16) + Utilities::GetDecimal(HexColorPtr[1]);
		vectorRGB[i] = IntColor;// / 255;
		//vectorRGB[i] = 80;
		HexColorPtr += 2;
	}
}

int Utilities::GetDecimal(char hexchar){
	if ((hexchar >= '0') && (hexchar <= '9')) return hexchar - '0';
	if ((hexchar >= 'A') && (hexchar <= 'F')) return hexchar - 'A' + 10;
	if ((hexchar >= 'a') && (hexchar <= 'f')) return hexchar - 'a' + 10;

	return -1; // Wrong character
}


float Utilities::pixel2unidadLogica(int pixel, int pixelsTotales, float uLogicasTotales){
	float aux = (float)pixel*(float)uLogicasTotales;
	return aux/(float)pixelsTotales;
}

int Utilities::unidadLogica2Pixel(float uLogica, int pixelsTotales, float uLogicasTotales){
	double aux = (float)uLogica*(float)pixelsTotales;
	return Utilities::redondeo((float)aux/(float)uLogicasTotales);
}

int Utilities::redondeo(float num){
	int aux = num;
	float dif = num - (float)aux;
	if(std::abs(dif) < 0.5f)
		return aux;
	else
		return aux<0?aux - 1:aux + 1;
}

float Utilities::grados2radianes(float grados){
	while( grados > 360 ){
		grados -= 360;
	}
	return (grados*PI/180.0f);
}

float Utilities::radianes2grados(float radianes){
	while( radianes > 2*PI ){
		radianes -= 2*PI;
	}
	return (radianes*180.0f/PI);
}

void Utilities::calcularVerticesPoligonoRegularFisico(float vertices_x[MAX_LADOS], float vertices_y[MAX_LADOS], int cantidadVertices, float radio, float angulo ){
	float variacion = Utilities::grados2radianes(360/cantidadVertices);
	float anguloAux = angulo;
	for( short i = 0; i < cantidadVertices ; i++ ){
		Utilities::calcularVerticePoligonoRegularFisico(vertices_x[i], vertices_y[i], radio, anguloAux );
		anguloAux += variacion;
	}
}

void Utilities::calcularVerticesPoligonoRegularVista(short vertices_x[MAX_LADOS], short vertices_y[MAX_LADOS], int cantidadVertices, int radio, float angulo ){
	float variacion = Utilities::grados2radianes(360/cantidadVertices);
	float anguloAux = angulo;
	for( short i = 0; i < cantidadVertices ; i++ ){
		Utilities::calcularVerticePoligonoRegularVista(vertices_x[i], vertices_y[i], radio, anguloAux );
		anguloAux += variacion;
	}
}

void Utilities::calcularVerticePoligonoRegularFisico(float &x, float &y, float radio, float angulo ){
		float radianes = angulo;
		x = cos(radianes)*radio;
		y = sin(radianes)*radio;
}

void Utilities::calcularVerticePoligonoRegularVista(short &x, short &y, int radio, float angulo ){
		float radianes = angulo;
		x = cos(radianes)*radio;
		y = sin(radianes)*radio;
}

void Utilities::calcularVerticesParalelogramoFisico(float vertices_x[4], float vertices_y[4], float x, float y,float base, float altura, float angulo_inf_izq, float rotacion ){
	// Un paralelogramo se define por su ancho, su alto y su angulo interno

	/* Ubicacion de los vertices:
	 V1 ______ V2 
	   /      / 
      /______/
	 V4      V3
	*/

	float base_triangulo = altura / tan(angulo_inf_izq);

	// V1
	vertices_x[0] = (-1) * ( (base - base_triangulo)/2 );
	vertices_y[0] = altura/2;

	// V2
	vertices_x[1] = vertices_x[0] + base;
	vertices_y[1] = vertices_y[0];

	// V3
	vertices_x[2] = vertices_x[1] - base_triangulo;
	vertices_y[2] = (-1) * vertices_y[1];

	// V4
	vertices_x[3] = vertices_x[2] - base;
	vertices_y[3] = vertices_y[2];

	//Utilities::rotarVertices( -1*rotacion , vertices_x, vertices_y, LADOS_PARALELOGRAMO, 0, 0);
}

void Utilities::calcularVerticesTrapecioFisico( float vertices_x[4], float vertices_y[4], float base_inferior, float altura, float angulo_interno_izq, float angulo_interno_der, float rotacion ){
	// Un trapecio se define por su base inferior, su alto y sus dos angulo internos inferiores (izquierdo y derecho)

	/* Ubicacion de los vertices:
	 V1 ______ V2 
	   /      \ 
      /________\
	 V4         V3
	*/

	float base_izq = altura / tan(angulo_interno_izq);
	float base_der = altura / tan(angulo_interno_der);

	// V1
	vertices_x[0] = (-1) * (base_inferior/2) + base_izq;
	vertices_y[0] = altura/2;

	// V2
	vertices_x[1] = vertices_x[0] + base_inferior - base_izq - base_der;
	vertices_y[1] = vertices_y[0];

	// V3
	vertices_x[2] = base_inferior/2; //vertices_x[1] + base_der;
	vertices_y[2] = (-1) * (altura/2);

	// V4
	vertices_x[3] = vertices_x[2] - base_inferior;
	vertices_y[3] = vertices_y[2];

	//Utilities::rotarVertices( -1*rotacion , vertices_x, vertices_y, LADOS_TRAPECIO, 0, 0);
}

void Utilities::calcularVerticesTrapecioVista( short vertices_x[4], short vertices_y[4], float base_inferior, float altura, float angulo_interno_izq, float angulo_interno_der, float rotacion ){
	// Un trapecio se define por su base inferior, su alto y sus dos angulo internos inferiores (izquierdo y derecho)

	/* Ubicacion de los vertices:
	 V1 ______ V2 
	   /      \ 
      /________\
	 V4         V3
	*/

	float base_izq = altura / tan(angulo_interno_izq);
	float base_der = altura / tan(angulo_interno_der);

	// V1
	vertices_x[0] = (-1) * (base_inferior/2) + base_izq;
	vertices_y[0] = (-1) * altura/2;

	// V2
	vertices_x[1] = vertices_x[0] + base_inferior - base_izq - base_der;
	vertices_y[1] = vertices_y[0];

	// V3
	vertices_x[2] = base_inferior/2; //vertices_x[1] + base_der;
	vertices_y[2] = (altura/2);

	// V4
	vertices_x[3] = vertices_x[2] - base_inferior;
	vertices_y[3] = vertices_y[2];

	Utilities::rotarVertices( rotacion , vertices_x, vertices_y, 4, 0, 0);
}

void Utilities::rotarVertices(float anguloRadianes, short vertices_x[MAX_LADOS], short vertices_y[MAX_LADOS], short cantidadVertices, short centro_x, short centro_y){
	while( anguloRadianes > 2*PI ){
		anguloRadianes -= 2*PI;
	}
	while( anguloRadianes < -2*PI ){
		anguloRadianes += 2*PI;
	}
	
	if( anguloRadianes ==  0 ) return;

	float aux_vertice_x, aux_vertice_y, vertice_rotado_x, vertice_rotado_y;
	float coseno_angulo = (float)cos(anguloRadianes);
	float seno_angulo   = (float)sin(anguloRadianes);

	// Centro de la figura en coordenadas euclideas
	centro_y = Utilities::ALTO_PX - centro_y; //SDL to Euclid

	//cout << "centro_x: " <<  centro_x << endl;
	//cout << "centro_y: " <<  centro_y << endl;

	for( short i = 0; i < cantidadVertices ; i++ ){
		aux_vertice_x = vertices_x[i];
		aux_vertice_y = Utilities::ALTO_PX - vertices_y[i]; //SDL to Euclid

		//cout << "x: " <<  vertices_x[i] << endl;
		//cout << "y: " <<  Utilities::ALTO_PX - vertices_y[i] << endl;

		aux_vertice_x -= centro_x;
		aux_vertice_y -= centro_y;

		//cout << "x centrado: " <<  aux_vertice_x << endl;
		//cout << "y centrado: " <<  aux_vertice_y << endl;

		vertice_rotado_x = (aux_vertice_x * coseno_angulo) - (aux_vertice_y * seno_angulo);
		vertice_rotado_y = (aux_vertice_x * seno_angulo) + (aux_vertice_y * coseno_angulo);

		//cout << "x centrado rotado: " <<  vertice_rotado_x << endl;
		//cout << "y centrado rotado: " <<  vertice_rotado_y << endl;

		vertice_rotado_x = Utilities::redondeo(vertice_rotado_x) + centro_x;
		vertice_rotado_y = Utilities::redondeo(vertice_rotado_y) + centro_y;

		//cout << "vertice_rotado_x: " <<  vertice_rotado_x << endl;
		//cout << "vertice_rotado_y: " <<  vertice_rotado_y << endl << endl;

		vertices_x[i] = vertice_rotado_x;
		vertices_y[i] = Utilities::ALTO_PX - vertice_rotado_y; //Euclid to SDL
	}
}

void Utilities::rotarVertices(float anguloRadianes, float vertices_x[MAX_LADOS], float vertices_y[MAX_LADOS], short cantidadVertices, short centro_x, short centro_y){
	while( anguloRadianes > 2*PI ){
		anguloRadianes -= 2*PI;
	}
	while( anguloRadianes < -2*PI ){
		anguloRadianes += 2*PI;
	}
	
	if( anguloRadianes ==  0 ) return;

	float aux_vertice_x, aux_vertice_y, vertice_rotado_x, vertice_rotado_y;
	float coseno_angulo = (float)cos(anguloRadianes);
	float seno_angulo   = (float)sin(anguloRadianes);

	// Centro de la figura en coordenadas euclideas
	centro_y = Utilities::ALTO_PX - centro_y; //SDL to Euclid

	//cout << "centro_x: " <<  centro_x << endl;
	//cout << "centro_y: " <<  centro_y << endl;

	for( short i = 0; i < cantidadVertices ; i++ ){
		aux_vertice_x = vertices_x[i];
		aux_vertice_y = Utilities::ALTO_PX - vertices_y[i]; //SDL to Euclid

		//cout << "x: " <<  vertices_x[i] << endl;
		//cout << "y: " <<  Utilities::ALTO_PX - vertices_y[i] << endl;

		aux_vertice_x -= centro_x;
		aux_vertice_y -= centro_y;

		//cout << "x centrado: " <<  aux_vertice_x << endl;
		//cout << "y centrado: " <<  aux_vertice_y << endl;

		vertice_rotado_x = (aux_vertice_x * coseno_angulo) - (aux_vertice_y * seno_angulo);
		vertice_rotado_y = (aux_vertice_x * seno_angulo) + (aux_vertice_y * coseno_angulo);

		//cout << "x centrado rotado: " <<  vertice_rotado_x << endl;
		//cout << "y centrado rotado: " <<  vertice_rotado_y << endl;

		vertice_rotado_x = vertice_rotado_x + centro_x;
		vertice_rotado_y = vertice_rotado_y + centro_y;

		//cout << "vertice_rotado_x: " <<  vertice_rotado_x << endl;
		//cout << "vertice_rotado_y: " <<  vertice_rotado_y << endl << endl;

		vertices_x[i] = vertice_rotado_x;
		vertices_y[i] = Utilities::ALTO_PX - vertice_rotado_y; //Euclid to SDL
	}
}



void Utilities::DrawBox2DBody(SDL_Renderer *buffer, b2Body *body, int fr, int fg, int fb, int falpha, int lr, int lg, int lb, int lalpha, bool aa){
    const b2Transform& xf = body->GetTransform();
    for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext())
    {
        switch (f->GetType())
        {
        case b2Shape::e_circle:
        {
            b2CircleShape* circle = (b2CircleShape*) f->GetShape();

            b2Vec2 center = b2Mul(xf, circle->m_p);
            float32 radius = circle->m_radius;
            //b2Vec2 axis = xf.R.col1;

            //m_debugDraw->DrawSolidCircle(center, radius, axis, color);
            if (falpha > 0)
            {
                filledCircleRGBA(buffer, center.x, Utilities::euclid2sdl_ejeY(center.y), (int) radius, fr, fg, fb, falpha);
            }
            if (lalpha > 0)
            {
                if (aa)
                {
                    aacircleRGBA(buffer, center.x, Utilities::euclid2sdl_ejeY(center.y), (int) radius, lr, lg, lb, lalpha);
                } else
                {
                    aacircleRGBA(buffer, center.x, Utilities::euclid2sdl_ejeY(center.y), (int) radius, lr, lg, lb, lalpha);
                }
            } else if (aa)
            {
                aacircleRGBA(buffer, center.x, Utilities::euclid2sdl_ejeY(center.y), (int) radius, fr, fg, fb, falpha);
            }

        }
            break;

        case b2Shape::e_polygon:
        {
            b2PolygonShape* poly = (b2PolygonShape*) f->GetShape();
			int32 vertexCount = poly->GetVertexCount();
			if(vertexCount == 5){
				return;
				//system("pause");
			}
            b2Assert(vertexCount <= b2_maxPolygonVertices);
            b2Vec2 vertices[b2_maxPolygonVertices];
            Sint16 xv[b2_maxPolygonVertices];
            Sint16 yv[b2_maxPolygonVertices];
            for (int32 i = 0; i < vertexCount; ++i)
            {
                vertices[i] = b2Mul(xf, poly->m_vertices[i]);
				xv[i] = (int) Utilities::redondeo(vertices[i].x);
				yv[i] = Utilities::euclid2sdl_ejeY(Utilities::redondeo(vertices[i].y));
            }
			//if(vertexCount == 4) printf("Rot: %f\n",body->GetAngle());
			//if(vertexCount == 4) for(int i = 0; i<vertexCount; i++) printf("FROM BOX2D: (%f,%f)\n", vertices[i].x, vertices[i].y);
			//if(vertexCount == 4) for(int i = 0; i<vertexCount; i++) printf("FROM BOX2D: (%f,%f)\n", poly->m_vertices[i].x, poly->m_vertices[i].y);
			//if(vertexCount == 4) for(int i = 0; i<vertexCount; i++) printf("FROM BOX2D: (%d,%d)\n", xv[i], yv[i]);
			//if(vertexCount == 4) cout << endl;
            if (falpha > 0)
            {
                filledPolygonRGBA(buffer, xv, yv, (Sint16) vertexCount, fr, fg, fb, falpha);
            }
            if (lalpha > 0)
            {
                if (aa)
                {
                    aapolygonRGBA(buffer, xv, yv, (Sint16) vertexCount, lr, lg, lb, lalpha);
                } else
                {
                    polygonRGBA(buffer, xv, yv, (Sint16) vertexCount, lr, lg, lb, lalpha);
                }
            } else if (aa)
            {
                aapolygonRGBA(buffer, xv, yv, (Sint16) vertexCount, fr, fg, fb, falpha);
            }
            //m_debugDraw->DrawSolidPolygon(vertices, vertexCount, color);
        }
            break;
        }
    }
}