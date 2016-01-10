/* Po³¹czenia KS0108 z AVR */
#define KS0108_RS C, 0
#define KS0108_RW C, 1
#define KS0108_E  C, 2
#define KS0108_CS1	C, 3
#define KS0108_CS2	C, 4
//#define KS0108_RESET	C, 5		//Nale¿y zdefiniowaæ jeœli u¿ywamy RESET

/* Linie danych D0-D7 musz¹ odpowiadaæ PORTx0-PORTx7 */
#define KS0108_DATA D

#define SLOW_TEXT 0		//Jeœli 1 to zostanie ¿yta wolniejsza procedura wyœwietlania tekstu
						//ale za to mo¿na wyœwietlaæ z dok³adnoœci¹ do pixela
