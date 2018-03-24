#include "StdAfx.h"
#include "TestFigurasCreacion.h"

void TestFigurasCreacion::test_CreacionDeCirculoYRectangulo(){
	b2Vec2 gravedad(0.0f, -10.0f);
	b2World mundo(gravedad);
	Circulo circulo(&mundo,0,50,10.0f,"Rojo",0,1,true);
	Rectangulo rect(&mundo,0,65,5.0f,5.0f,"Rojo",0,1,true);

	//a static floor to drop things on
	b2Vec2 v1(-10.0f, 0.0f);
	b2Vec2 v2(10.0f, 0.0f);
	b2EdgeShape edge;
	edge.Set(v1, v2);

	b2BodyDef pisoDef;
	pisoDef.position.Set(0.0f,0);
	pisoDef.type=b2_staticBody;
	b2Body* piso=mundo.CreateBody(&pisoDef);

	b2FixtureDef pisoFixDef;
	pisoFixDef.shape=&edge;
	pisoFixDef.restitution=0;
	piso->CreateFixture(&pisoFixDef);
	
	
	float32 timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 8;
	int32 positionIterations = 3;
	
	for (int32 i = 0; i < 10000; ++i){
		mundo.Step(timeStep, velocityIterations, positionIterations);

		printf("Circulo X: %4.2f Y:%4.2f\n", circulo.getX(), circulo.getY());
		printf("Rectang X: %4.2f Y:%4.2f\n\n", rect.getX(), rect.getY());

	}

	cin.get();
}