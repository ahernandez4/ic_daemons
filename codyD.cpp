/* Cody Davis
 * CMPS 3350
 * 
*/

//includes for colro scenes
#include <GL/glx.h>


#include "fonts.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
//below are all the inlcusions for odinGetTime() and anything needed
//for establishing connections or the code used in general
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <string.h>
#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <unistd.h>

#define USERAGENT "CMPS-3350"
#define PORT 443

const int MAX_READ_ERRORS = 100;
//inlcudes for odinGetTime() end here
extern int playTime(int x, int y);


//funciton for changing color in scene for day night cycle

int changingSceneColor(int color) { 
     
/*
     //uses incrementor from main to cycle through day night
     if ((incrementor % 200) == 0) {
	   if (color != 6) {
		color++;
	   }
	   else {
		color = 0;
	   }
     }  
*/
     //this if all colors for day night cycle
     //clear
     if (color == 0) {
         glColor3f(1.0, 1.0, 1.0);
         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
         
     }
     //yellow
     else if (color == 1) {
         glColor3f(1.0f, 1.0f, 0.0f);
         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     }
     //purple
     else if (color == 3) {
         glColor3f(0.5f, 0.0f, 1.0f);
         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     }
     //blue
     else if (color == 4) {
         glColor3f(0.0f, 0.0f, 1.0f);
         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     }
        
     //purple
     else if (color == 4) {
         glColor3f(0.5f, 0.0f, 1.0f);
         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     }
     //yellow
     else if (color == 6) {
         glColor3f(1.0f, 1.0f, 0.0f);
         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     }

     return color;
}


//This is mh Friday code
//~~~~~~~~
//~~~~~~~~
//this function is used for putting the minutes played to screen 
void displayTimeUI(int displayTime, int displayCredits) {	
    Rect r;
    int x = 15;
    int y = 484;
    int advance = 0;
    r.bot = y;
    r.left = x;
    r.center = 0;
    unsigned int color = 0x00ffff44; 
    
    if (!displayTime && !displayCredits) {	
    	ggprint8b(&r, advance, color, "press \"t\" to display time");
    }    
    if (displayTime && !displayCredits) {	
	playTime(15, 484);
    }
}


//this is for the timePlayD
int odinGetTime();

/* function: displayCD(int x, int y) 
 * description: meant to display name using ggprint8b
 *      x and y input is given to center the box in
 *      the desired location. see ggprint8b for more
 *      information on proper use.
 */

void displayCD(int x, int y)
{
    Rect r;
    int advance = 48;
    r.bot = y;
    r.left = x;
    r.center = 0;
    unsigned int color = 0x00ffff44; 
    
    ggprint8b(&r, advance, color, "Cody Davis");
}


/* This fucntion is for displaying time to screen after pulling
 * current runtime from the server through the odinGetTime()
 * function is called and returns the value from the server.
 *
 * Input: takes in x and y coordinates that provide the location for
 * the drawing to screen.
 * 
 * Output: no ouput it just displays the text to screen and exits
 */

void displayOdinTime()
{
    Rect r;
    int advance = 0;
    r.bot = 484;
    r.left = 10;
    r.center = 0;
    int time = 0;
    unsigned int color = 0xFEEDFEED;
    time = odinGetTime();
    
    ggprint8b(&r, advance, color, "Current time played: %d min.", time );   
}


/* this function gets time from the odin server and uses PHP to access
 * a text file on the public HTML.
 *
 * 
 *
 * int: the output "t" is a time for the time played that is currently on the 
 * txt in the public HTML.
*/

int odinGetTime(){
    BIO *ssl_setup_bio(void);

    //void show_cert_data(SSL *ssl, BIO *outbio, const char *hostname);
    void set_to_non_blocking(const int sock);
    int sd;
    struct hostent *host;
    struct sockaddr_in addr;
    BIO *outbio = NULL;

    // fixed warning: invalid conversion from 'const SSL_METHOD*
    // added const
    const SSL_METHOD *method;
    SSL_CTX *ctx;
    SSL *ssl;
    char req[1000];
    int req_len;

    char hostname[256] = "odin.cs.csub.edu";
    char pagename[256] = "/~ahernandez2/3350/game/highscores.php";

    int port = PORT;
    int bytes, nreads, nerrs;
    char buf[256];
    int ret;

    outbio = ssl_setup_bio();

    //Initialize the SSL library
    if (SSL_library_init() < 0)
        BIO_printf(outbio, "Could not initialize the OpenSSL library !\n");
    method = SSLv23_client_method();
    ctx = SSL_CTX_new(method);
    SSL_CTX_set_options(ctx, SSL_OP_NO_SSLv2);

    //next 2 lines of code are not currently needed.
    //SSL_MODE_AUTO_RETRY flag of the SSL_CTX_set_mode call.
    //SSL_CTX_set_mode(ctx, SSL_MODE_AUTO_RETRY);
    //Setup the socket used for connection.
    host = gethostbyname(hostname);
    sd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = *(long*)(host->h_addr);

    if (connect(sd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
       // BIO_printf(outbio, "%s: Cannot connect to host %s [%s] on port %d.\n", 
       // argv[0], hostname, inet_ntoa(addr.sin_addr), port);
    }

    //Connect using the SSL certificate.
    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, sd);
    SSL_connect(ssl);

    set_to_non_blocking(sd);

    //Send the http request to the host server.
    sprintf(req, "GET /%s HTTP/1.1\r\nHost: %s\r\nUser-Agent: %s\r\n\r\n",
            pagename, hostname, USERAGENT);
    req_len = strlen(req);
    ret = SSL_write(ssl, req, req_len);
    if (ret <= 0) {
        fprintf(stderr, "ERROR: SSL_write\n");
    }

    fflush(stderr);

    //Get data returned from the server.
    //First, do priming read.
    //We can take this approach because our socket is non-blocking.
    //Start with an error condition.
    bytes = -1;
    memset(buf, '\0', sizeof(buf));
    while (bytes <= 0) {
        bytes = SSL_read(ssl, buf, sizeof(buf));
        //A slight pause can cause fewer reads to be needed.
        usleep(10000);
    }

    //A successful priming read was accomplished.
    //Now read all the data.
    nreads = 1;

    //Allow for some read errors to happen, while getting the complete data.
    nerrs = 0;
    char timeP[2];
    int timeNum = 0;
    while (bytes >= 0 && nerrs < MAX_READ_ERRORS) {
        for(int i = 0; i < 255; i++) {
            if(buf[i] == '*') {
                timeP[0] = buf[i + 1];
                timeP[1] = buf[i + 2];
                break;
            }
        }
        sscanf(timeP, "%d", &timeNum);

        memset(buf, '\0', sizeof(buf));
        ++nreads;
        bytes = SSL_read(ssl, buf, sizeof(buf));
        if (bytes == 0)
            ++nerrs;
        else
            nerrs = 0;
        //A slight pause can cause fewer reads to be needed.
        usleep(20000);
    }
       
    SSL_free(ssl);
    close(sd);
    SSL_CTX_free(ctx);
    int t = atoi(timeP);
    
    return t;
}


