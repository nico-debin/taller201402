#ifndef _Constantes_H_
#define _Constantes_H_

#define NOMBRE_JUEGO "SnowBross V1"

#define SERVIDOR_PUERTO_DEFAULT 8200
#define CLIENTE_PUERTO_DEFAULT  8200

#define PI 3.141592654f

// Modelos de figuras
#define CIRCULO "circulo"
#define RECTANGULO "rect"
#define POLIGONO "poli"
#define TRIANGULO "triangulo"
#define CUADRADO "cuadrado"
#define PENTAGONO "pentagono"
#define HEXAGONO "hexagono"
#define PARALELOGRAMO "paralelogramo"
#define TRAPECIO "trapecio"

enum TIPO_FIGURA {
			TIPO_CIRCULO = 1,
			TIPO_RECTANGULO,
			TIPO_POLIGONO,
			TIPO_TRIANGULO,
			TIPO_CUADRADO,
			TIPO_PENTAGONO,
			TIPO_HEXAGONO,
			TIPO_PARALELOGRAMO,
			TIPO_TRAPECIO,
			TIPO_SNOWBOY,
			TIPO_NULL
					};

#define LADOS_TRIANGULO 3
#define LADOS_CUADRADO 4
#define LADOS_PENTAGONO 5
#define LADOS_HEXAGONO 6
#define LADOS_PARALELOGRAMO 4
#define LADOS_TRAPECIO 4
#define MAX_LADOS 6 // El poligono de mayor lados



// Defaults
#define PATH_ESCENARIO_DEFAULT "config\\escenario_default.json"
#define EXTENSION ".json"
#define IMAGEN_FONDO_DEFAULT "imagenes/fondoDefault.png"
#define MAX_JUGADORES 1 // Maximos clientes que acepta el servidor

#define FPS 30

#define ALTO_DEFAULT_PX 600
#define ANCHO_DEFAULT_PX 800
#define ALTO_DEFAULT_UL 100
#define ANCHO_DEFAULT_UL 100

// Definicion de eventos recibidos por la vista (cliente)
//#define EVENTO_CREAR_ESCENARIO_1 100
//
//#define EVENTO_CREAR_CIRCULO 200
//#define EVENTO_CREAR_TRIANGULO 201
//#define EVENTO_CREAR_RECTANGULO 202
//#define EVENTO_CREAR_PENTAGONO 203
//#define EVENTO_CREAR_HEXAGONO 203
//#define EVENTO_CREAR_PARALELOGRAMO 204
//#define EVENTO_CREAR_TRAPECIO 205
//
//#define EVENTO_ACTUALIZAR_FIGURA 300

//Simulacion del paso del tiempo Box2D
//const float PASO_TIEMPO = 1.0f / 60.0f;
const float PASO_TIEMPO = 1.0f / 150.0f;
const int ITERACION_VELOCIDAD = 8;
const int ITERACION_POSICION = 8;

//gravedad del mundo Box2D
const float GRAVEDAD_X = 0.0f;
const float GRAVEDAD_Y = -9.80f;

const float FRICCION = 0.2f;
const float RESTITUCION = 0.2f;

// vertices de la figuras
static const int CANTIDADVERTICES = 4;

//Eventos recibidos por la vista/cliente
enum EVENTOS_RECIBIDOS_CLIENTE { 
					EVENTO_CLIENTE_JUEGO_RESET = 0,
					EVENTO_CLIENTE_JUEGO_SALIR,
					EVENTO_CLIENTE_JUEGO_PAUSA,
					EVENTO_CLIENTE_JUEGO_STEP,
					EVENTO_CLIENTE_FIGURA_ACTUALIZAR,
					EVENTO_CLIENTE_PERSONAJE_NUEVO_ID,
					EVENTO_CLIENTE_PERSONAJE_CREADO, 
					EVENTO_CLIENTE_PERSONAJE_QUIETO, 
					//EVENTO_CLIENTE_PERSONAJE_CAMINANDO, 
					EVENTO_CLIENTE_PERSONAJE_CAMINANDO_IZQ,
					EVENTO_CLIENTE_PERSONAJE_CAMINANDO_DER,

					EVENTO_CLIENTE_PERSONAJE_SALTANDO, 
					EVENTO_CLIENTE_PERSONAJE_SALTANDO_IZQ, 
					EVENTO_CLIENTE_PERSONAJE_SALTANDO_DER,

					EVENTO_CLIENTE_PERSONAJE_MURIENDO,
					EVENTO_CLIENTE_PERSONAJE_MUERTO,
					EVENTO_CLIENTE_PERSONAJE_DESCONECTADO,
					EVENTO_CLIENTE_PERSONAJE_LANZANDO,
					EVENTO_CLIENTE_PERSONAJE_EMPUJANDO,

					EVENTO_CLIENTE_CONEXION_ACEPTADA,
					EVENTO_CLIENTE_RECONEXION_ACEPTADA,
					EVENTO_CLIENTE_CONEXION_RECHAZADA_SALA_LLENA,
					EVENTO_CLIENTE_CONEXION_RECHAZADA_NOMBRE_EXISTENTE,
					EVENTO_CLIENTE_NULL
					};

//Eventos recibidos por juegoControlador/servidor
enum EVENTOS_RECIBIDOS_SERVIDOR { 
	                    EVENTO_SERVIDOR_SALIR = 0,
						EVENTO_SERVIDOR_PAUSA,
						EVENTO_SERVIDOR_RESET,

                        EVENTO_SERVIDOR_SALTAR_PERSONAJE,
                        EVENTO_SERVIDOR_MOVER_IZQ_PERSONAJE,
                        EVENTO_SERVIDOR_MOVER_DER_PERSONAJE,
											
						EVENTO_SERVIDOR_FRENAR_DER_PERSONAJE,
						EVENTO_SERVIDOR_FRENAR_IZQ_PERSONAJE,

						EVENTO_SERVIDOR_CREAR_SNOWBOY,

						EVENTO_SERVIDOR_NULL
					};

//SnowBoy
#define SNOWBOY_ESCALA 1.2 

#define SNOWBOY_ALTO_PX 30
#define SNOWBOY_ANCHO_PX 20


#define RADIO_CUERPO  (1.3f*SNOWBOY_ESCALA)

#define SNOWBOY_FLOTACION_DESEADA (RADIO_CUERPO*2)//1.05F 

#define ALTURA_INICIAL_PIERNAS 0//(RADIO_CUERPO/-2)//-0.5f
#define ALTURA_FINAL_PIERNAS (-SNOWBOY_FLOTACION_DESEADA/*-0.5*/)//-2.0f
#define ANCHO_PIERNAS (RADIO_CUERPO/4)

#define ANGULO_MAX_ESCALABLE 55

#define RAYO_HORIZONTAL_POSX (RADIO_CUERPO+0.3f)
#define RAYO_HORIZONTAL_POSY 0

#define VELOCIDAD_HORIZONTAL 10//5
#define VELOCIDAD_SALTO 10
#define SNOWBOY_CONSTANTE_DESACELERACION 0.25

#define SNOWBOY_ALTO_UL (SNOWBOY_FLOTACION_DESEADA+RADIO_CUERPO)
#define SNOWBOY_ANCHO_UL (RADIO_CUERPO*2)

#define SNOWBOY_DIFERENCIA_CENTRO_CUERPO_CENTRO_FIGURA (SNOWBOY_FLOTACION_DESEADA-SNOWBOY_ALTO_UL/2)


#define PERSONAJE_VISTA_CLIP_COUNT 30//50

////Estados comunes a todos los PERSONAJES son utilizados solo por la vista
//enum ESTADO_PERSONAJE { CREADO = 0, 
//						QUIETO, 
//					    CAMINANDO, 
//					    SALTANDO, 
//					    MURIENDO,
//					    MUERTO
//					};
//
//// Estados exclusivos del SNOWBOY son utilizados solo por la vista
//enum ESTADO_SNOWBOY { LANZANDO_NIEVE = 7, 
//					  EMPUJANDO
//					};

//Constantes del Zoom
#define SNOWBOY_PROPORCION_SCROLL_X_EN_PANTALLA_MIN 0.25f //Al scrollear el SnowBoy va a estar en 1/2 de la pantalla desde la izq. Si fuera 0.25f estaria en 1/4 de la pantalla desde la izq
#define SNOWBOY_PROPORCION_SCROLL_Y_EN_PANTALLA 0.75f //Al scrollear el SnowBoy va a estar en 3/4 de la pantalla desde arriba. Si fuera .025f estaria en 1/4 de la pantalla desde arriba   
#define ZOOM_INICIAL 1.5f
#define ZOOM_MAXIMO 4.1f
#define ZOOM_VARIACION 0.1f




#endif