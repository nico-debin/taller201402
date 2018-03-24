#include "StdAfx.h"
#include "Rectangulo.h"


Rectangulo::Rectangulo(b2World* mundo,sObjeto structObj):Figura(mundo,structObj){
	//mundo->SetContactListener(this);
	b2PolygonShape rectanguloShape;
	rectanguloShape.SetAsBox(this->datos.ancho/2 , this->datos.alto/2);
	
	b2FixtureDef rectanguloFixDef;
	rectanguloFixDef.shape = &rectanguloShape; 
	rectanguloFixDef.friction = FRICCION;
	rectanguloFixDef.restitution = RESTITUCION; //para establecer cuánto rebota
	rectanguloFixDef.density=calcularDensidad();
	this->fixtureDef=body->CreateFixture(&rectanguloFixDef);

	this->datos.lados = LADOS_CUADRADO;
}

b2Fixture* Rectangulo::getFixture(){
	return this->fixtureDef;
}

//Rectangulo::Rectangulo():Figura(){}


float Rectangulo::calcularArea(){
	return this->getBase()*this->getAltura();
}
/**
void Rectangulo::BeginContact(b2Contact* contact) {

    if (contact->GetFixtureA()->GetBody() == this->getFiguraCuerpo() || contact->GetFixtureB()->GetBody() == this->getFiguraCuerpo() ){
        
    }
}

void Rectangulo::EndContact(b2Contact* contact) {

  if (contact->GetFixtureA()->GetBody() == this->getFiguraCuerpo() || contact->GetFixtureB()->GetBody() == this->getFiguraCuerpo())
    {
        body->SetLinearVelocity(b2Vec2(0, -10));
        
    }
	
}
*/

float Rectangulo::getBase(){
	return this->datos.ancho;
}

float Rectangulo::getAltura(){
	return this->datos.alto;
}


void Rectangulo::getVertices(float vertices_x[4], float vertices_y[4]){
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