#include "StdAfx.h"
#include "Trapecio.h"


Trapecio::Trapecio(b2World* mundo,sObjeto structObj):Figura(mundo,structObj){
	this->datos.lados = LADOS_TRAPECIO;
	mundo->SetContactListener(this);

	this->datos.angulo_1 = Utilities::grados2radianes(structObj.angulo_1);
	this->datos.angulo_2 = Utilities::grados2radianes(structObj.angulo_2);

	this->calcularVertices();
	this->getFormaCuerpo();
}

void Trapecio::calcularVertices(){
	// Un trapecio se define por su base inferior, su alto y sus dos angulo internos inferiores (izquierdo y derecho)
	Utilities::calcularVerticesTrapecioFisico( this->verticesX, this->verticesY, this->datos.ancho, this->datos.alto, this->datos.angulo_1, this->datos.angulo_2, this->datos.rot );	
}

void Trapecio::getFormaCuerpo(void){
		b2Vec2 puntos[LADOS_TRAPECIO];	
	puntos[0].Set(verticesX[0], verticesY[0]);
	puntos[1].Set(verticesX[1], verticesY[1]);
	puntos[2].Set(verticesX[2], verticesY[2]);
	puntos[3].Set(verticesX[3], verticesY[3]);

	b2PolygonShape trapecioShape;
	trapecioShape.Set(puntos,4);

	b2FixtureDef fix;
	fix.shape = &trapecioShape;
	fix.restitution = RESTITUCION; //para establecer cuánto rebota
	fix.friction = FRICCION;
	fix.density = this->calcularDensidad();
	this->fixtureDef=body->CreateFixture(&fix);
}

b2Fixture* Trapecio::getFixture(){
	return this->fixtureDef;
}

float Trapecio::calcularArea(){
	return 10;
	//return ((this->datos.baseMayor+this->datos.baseMenor)*this->datos.alto)/2;
}

float Trapecio::getAnguloIzq(){
	return this->datos.angulo_1;
}

float Trapecio::getAnguloDer(){
	return this->datos.angulo_2;
}

void Trapecio::getVertices(float vertices_x[4], float vertices_y[4]){
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