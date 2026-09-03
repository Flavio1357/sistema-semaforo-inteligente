#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define LARGURA 128
#define ALTURA 64
#define OLED_RESET -1

Adafruit_SSD1306 display(LARGURA, ALTURA, &Wire, OLED_RESET);

const int pinCarroVermelho = 8;
const int pinCarroAmarelo = 9;
const int pinCarroVerde = 10;

const int pinPedestreVerde = 11;
const int pinPedestreVermelho = 12;

const int pinTrig = 5;
const int pinEcho = 6;

const int pinBuzzer = 7;

const int VERDE = 0;
const int AMARELO = 1;
const int VERMELHO = 2;

int estadoAtual = VERDE;

unsigned long inicioEstado = 0;
unsigned long fimTravessia = 0;
unsigned long ultimoBip = 0;
unsigned long ultimoOLED = 0;

bool bipAtivo = false;

const unsigned long TEMPO_AMARELO = 10000UL;
const unsigned long TEMPO_VERMELHO = 30000UL;
const unsigned long INTERVALO_PEDIDO = 30000UL;

const float DISTANCIA_ACIONAMENTO = 3.0;

float lerDistancia() {
	digitalWrite(pinTrig, LOW);
	delayMicroseconds(2);

	digitalWrite(pinTrig, HIGH);
	delayMicroseconds(10);

	digitalWrite(pinTrig, LOW);

	long duracao = pulseIn(pinEcho, HIGH, 30000);

	if (duracao == 0) {
		return 999.0;
	}

	float distancia = (duracao * 0.0343) / 2.0;

	return distancia;
}

bool podeFazerPedido() {
	unsigned long agora = millis();

	return agora - fimTravessia >= INTERVALO_PEDIDO;
}

void mostrarTexto(String linha1, String linha2) {
	display.clearDisplay();
	display.setTextColor(SSD1306_WHITE);
	display.setTextSize(2);

	display.setCursor(0, 5);
	display.println(linha1);

	display.setCursor(0, 35);
	display.println(linha2);

	display.display();
}

void mostrarContagem(String titulo, int segundos) {
	display.clearDisplay();
	display.setTextColor(SSD1306_WHITE);

	display.setTextSize(2);
	display.setCursor(0, 0);
	display.println(titulo);

	display.setTextSize(3);
	display.setCursor(35, 30);

	if (segundos < 10) {
		display.print("0");
	}

	display.print(segundos);
	display.print("s");

	display.display();
}

void mudarEstado(int novoEstado) {
	estadoAtual = novoEstado;
	inicioEstado = millis();

	digitalWrite(pinCarroVermelho, LOW);
	digitalWrite(pinCarroAmarelo, LOW);
	digitalWrite(pinCarroVerde, LOW);

	digitalWrite(pinPedestreVerde, LOW);
	digitalWrite(pinPedestreVermelho, LOW);

	noTone(pinBuzzer);

	bipAtivo = false;
	ultimoBip = millis();

	display.clearDisplay();
	display.display();

	if (estadoAtual == VERDE) {
		digitalWrite(pinCarroVerde, HIGH);
		digitalWrite(pinPedestreVermelho, HIGH);

		mostrarTexto("CARROS", "SIGA");
	}

	else if (estadoAtual == AMARELO) {
		digitalWrite(pinCarroAmarelo, HIGH);
		digitalWrite(pinPedestreVermelho, HIGH);

		mostrarTexto("ATENCAO", "PARE");
	}

	else if (estadoAtual == VERMELHO) {
		digitalWrite(pinCarroVermelho, HIGH);
		digitalWrite(pinPedestreVerde, HIGH);

		mostrarTexto("PEDESTRE", "SIGA");
	}
}

void setup() {
	pinMode(pinCarroVermelho, OUTPUT);
	pinMode(pinCarroAmarelo, OUTPUT);
	pinMode(pinCarroVerde, OUTPUT);

	pinMode(pinPedestreVerde, OUTPUT);
	pinMode(pinPedestreVermelho, OUTPUT);

	pinMode(pinTrig, OUTPUT);
	pinMode(pinEcho, INPUT);

	pinMode(pinBuzzer, OUTPUT);

	if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
		while (true) {
		}
	}

	display.clearDisplay();
	display.setTextColor(SSD1306_WHITE);
	display.setTextSize(2);

	display.setCursor(10, 10);
	display.println("SISTEMA");

	display.setCursor(10, 35);
	display.println("INICIADO");

	display.display();

	delay(2000);

	fimTravessia = millis() - INTERVALO_PEDIDO;

	mudarEstado(VERDE);
}

void loop() {
	unsigned long agora = millis();

	if (estadoAtual == VERDE) {
		if (podeFazerPedido()) {
			float distancia = lerDistancia();

			if (distancia > 0 && distancia <= DISTANCIA_ACIONAMENTO) {
				mudarEstado(AMARELO);
				return;
			}
		}
	}

	else if (estadoAtual == AMARELO) {
		unsigned long decorrido = agora - inicioEstado;

		if (decorrido >= TEMPO_AMARELO) {
			mudarEstado(VERMELHO);
			return;
		}

		int segundosRestantes = 10 - (decorrido / 1000);

		if (segundosRestantes < 0) {
			segundosRestantes = 0;
		}

		if (agora - ultimoOLED >= 200) {
			ultimoOLED = agora;

			mostrarContagem(
				"ATENCAO",
				segundosRestantes
			);
		}
	}

	else if (estadoAtual == VERMELHO) {
		unsigned long decorrido = agora - inicioEstado;

		if (decorrido >= TEMPO_VERMELHO) {
			noTone(pinBuzzer);

			mudarEstado(VERDE);

			fimTravessia = millis();

			return;
		}

		int segundosRestantes = 30 - (decorrido / 1000);

		if (segundosRestantes < 0) {
			segundosRestantes = 0;
		}

		if (agora - ultimoOLED >= 200) {
			ultimoOLED = agora;

			mostrarContagem(
				"PEDESTRE",
				segundosRestantes
			);
		}

		if (agora - ultimoBip >= 500) {
			ultimoBip = agora;

			bipAtivo = !bipAtivo;

			if (bipAtivo) {
				tone(pinBuzzer, 1000);
			}
			else {
				noTone(pinBuzzer);
			}
		}
	}
}