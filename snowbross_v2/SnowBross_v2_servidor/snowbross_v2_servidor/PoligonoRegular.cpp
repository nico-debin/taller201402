#include "StdAfx.h"
#include "PoligonoRegular.h"


PoligonoRegular::PoligonoRegular(b2World* mundo, float x,float y, int lados,float escala, string color,float rotacion, float masa, bool estatico):Figura(mundo,x,y,color,rotacion,estatico){

	this->datos.lados = lados;
	this->datos.escala = escala;

	this->datos.ancho = escala*2;
	this->datos.alto = this->datos.ancho;

	b2Vec2* vertices = new b2Vec2[lados];

	this->cargarVertices(vertices);
	b2PolygonShape poligonoShape;
	poligonoShape.Set(vertices,lados);

	b2FixtureDef fix;
	fix.shape = &poligonoShape;
	fix.friction = FRICCION;
	fix.restitution = RESTITUCION; //para establecer cuánto rebota
	fix.density = calcularDensidad();
	
	body->CreateFixture(&fix);
		
	delete[] vertices;

}

b2Fixture* PoligonoRegular::getFixture(){
	return this->fixtureDef;
}

PoligonoRegular::PoligonoRegular(b2World* mundo,sObjeto structObj):Figura(mundo,structObj){
	this->datos.ancho = structObj.escala*2;
	this->datos.alto = this->datos.ancho;

	b2Vec2 vertices[MAX_LADOS];
	mundo->SetContactListener(this);
	this->cargarVertices(vertices);

	b2PolygonShape poligonoShape;
	poligonoShape.Set(vertices, this->datos.lados);
	
	b2FixtureDef fix;
	fix.shape = &poligonoShape;
	fix.friction = FRICCION;
	fix.restitution = RESTITUCION; //para establecer cuánto rebota
	fix.density = calcularDensidad();
	this->fixtureDef= body->CreateFixture(&fix);
}

void PoligonoRegular::cargarVertices(b2Vec2* vertices){

	float angulo = 0.0f;
	float variacion = 360/this->datos.lados;

	for (int32 i = 0; i < this->datos.lados; ++i )
	{
		float radianes=gradosARadianes(angulo);
		vertices[i].Set( cos(radianes)*this->datos.escala , sin(radianes)*this->datos.escala );
		angulo += variacion;
	}

}


float PoligonoRegular::calcularArea(){
	float angulo = gradosARadianes(360/this->datos.lados);
	float mediaBase = sin(angulo/2)*this->datos.escala;
	float altura = cos(angulo/2)*this->datos.escala;	

	float areaTriangulo = (mediaBase*2)*altura/2;
	return (areaTriangulo*this->datos.lados);
}

//void PoligonoRegular::getVertices(float vertices_x[MAX_LADOS], float vertices_y[MAX_LADOS]){
void PoligonoRegular::getVertices(float vertices_x[MAX_LADOS], float vertices_y[MAX_LADOS]){
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
