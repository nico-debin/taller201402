#include "StdAfx.h"
#include "Paralelogramo.h"


Paralelogramo::Paralelogramo(b2World* mundo, float x,float y, float base,float altura, float anguloInclinacion, string color,float rotacion, float masa, bool estatico):Figura(mundo,x,y,color,rotacion,estatico){
	
	this->datos.lados = LADOS_PARALELOGRAMO;
	this->datos.baseMenor = base;
	this->datos.baseMayor = base;
	this->datos.alto = altura;
	this->datos.rot = rotacion;
	this->datos.angulo_1 = anguloInclinacion;
	this->datos.masa = masa;

	this->calcularVertices(x,y);
	this->getFormaCuerpo();
}

Paralelogramo::Paralelogramo(b2World* mundo,sObjeto structObj):Figura(mundo,structObj){
	this->datos.lados = LADOS_PARALELOGRAMO;
	this->datos.angulo_1 = Utilities::grados2radianes(structObj.angulo_1);

	mundo->SetContactListener(this);
	this->calcularVertices(this->datos.x, this->datos.y);
	this->getFormaCuerpo();
}

void Paralelogramo::calcularVertices(float x,float y){
	calcularVerticesParalelogramoFisico(this->verticesX, this->verticesY, x, y,this->datos.ancho, this->datos.alto, this->datos.angulo_1, this->datos.rot );
}

void Paralelogramo::calcularVerticesParalelogramoFisico(float vertices_x[4], float vertices_y[4], float x, float y,float base, float altura, float angulo_inf_izq, float rotacion ){
	// Un paralelogramo se define por su ancho, su alto y su angulo interno

	/* Ubicacion de los vertices:
	 V1 ______ V2 
	   /      / 
      /______/
	 V4      V3
	*/

	float base_triangulo = altura / tan(angulo_inf_izq);
	printf("alto: %.3f  ancho: %.3f", altura,base);
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

	
	this->datos.ancho = base + base_triangulo;
}






void Paralelogramo::getFormaCuerpo(void){

	b2PolygonShape paralelogramoShape;

	b2Vec2 puntos[LADOS_PARALELOGRAMO];	
	puntos[0].Set(verticesX[0], verticesY[0]);
	puntos[1].Set(verticesX[1], verticesY[1]);
	puntos[2].Set(verticesX[2], verticesY[2]);
	puntos[3].Set(verticesX[3], verticesY[3]);
	paralelogramoShape.Set(puntos, LADOS_PARALELOGRAMO);
		
	b2FixtureDef fix;
	fix.shape = &paralelogramoShape;
	fix.friction = FRICCION;
	fix.restitution = RESTITUCION; //para establecer cuánto rebota
	fix.density = 5;//calcularDensidad();
	this->fixtureDef= body->CreateFixture(&fix);
}

b2Fixture* Paralelogramo::getFixture(){
	return this->fixtureDef;
}

float Paralelogramo::calcularArea(){
	//Area = base*altura
	return this->getBase() * this->getAltura();
}

float Paralelogramo::getAnguloInclinacion(){
	return this->datos.angulo_1;
}

float Paralelogramo::getRotacionInicial(){
	return this->datos.rot;
}

float Paralelogramo::getBase(){
	return this->datos.baseMenor;
}

float Paralelogramo::getAltura(){
	return this->datos.alto;
}

void Paralelogramo::getVertices(float vertices_x[4], float vertices_y[4]){
	//const b2Transform& xf = this->body->GetTransform();
	/*b2PolygonShape* poly = (b2PolygonShape*) this->body->GetFixtureList()->GetShape();
	b2Vec2 vertices[4];
    for( short i = 0; i < 4; ++i ){
        vertices[i] = b2Mul(xf, poly->m_vertices[i]);
		vertices_x[i] = vertices[i].x;
		vertices_y[i] = vertices[i].y;
    }*/

	b2PolygonShape *poly = (b2PolygonShape*)this->body->GetFixtureList()->GetShape();
	
	//const int count = poly->GetVertexCount();
	
	for( int i = 0; i < poly->GetVertexCount(); i++ ){
		//b2Vec2 bcVertex = poly->GetVertex( i );
		b2Vec2 pos = this->body->GetFixtureList()->GetBody()->GetWorldPoint( poly->GetVertex( i ) );

		vertices_x[i] = pos.x;
		vertices_y[i] = pos.y;
	}
}
