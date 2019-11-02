//Tawfic Jobah
//My project file
//4 steps to debugging 
//recongnize the bug
//isolate
//identify
//fix
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "fonts.h"
#include <iostream>
#include <fstream>
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

extern unsigned char maparray[150][250];
struct enem{
    int x;
    int y;
    int moveSpeed;
    int health;

}en;

bool collision(int x, int y)
{
    if (maparray[x][y] == '2') {
        std::cout << maparray[x][y] << std::endl;
        return true;
    }
    else 
        return false;
}
void enemy()
{
    float r = 0.0f; //RED
    float g = 0.0f; //GREEN
    float b = 0.0f; //BLUE

    //glClear(GL_COLOR_BUFFER_BIT);
    //glClearColor(r, g, b, 255.0f);

    glBegin(GL_QUADS);
    glColor3f(0.0, 1.0, 0.0); //You can set RGB color for you vertex
    glVertex2f(-0.5f, 0.5f);
    glVertex2f(0.5f, 0.5f);
    glVertex2f(0.5f, -0.5f);
    glVertex2f(-0.5f, -0.5f);
    glEnd();

}

void tjcredits(int x, int y) //, GLuint texid)
{
    Rect r;
    r.bot = y;
    r.left = x;
    r.center = 0;
    ggprint8b(&r, 16, 0x00ffff44, "Tawfic Jobah");
}
void createEnemy()
{
    en.x = 220;
    en.y = 80;
    en.moveSpeed = 15;
    en.health = 3;
}

void updatingTime(int time)
{
    //deletes file, to rewrite so it can be updated
    if( remove( "highscores.txt" ) != 0 )
        std::cout << "error deleting file\n";
    else {
        std::ofstream myfile ("highscores.txt");
        if (myfile.is_open()) {
            myfile << '*';
            myfile << time;
            myfile.close();
        }
        else
            std::cout << "unable to open file\n";
    }
}
void odinPushTime(int time)
{
    BIO *ssl_setup_bio(void);
    //void show_cert_data(SSL *ssl, BIO *outbio, const char *hostname);
    void set_to_non_blocking(const int sock);
    int sd;
    struct hostent *host;
    struct sockaddr_in addr;
    BIO *outbio = NULL;
    // added const
    const SSL_METHOD *method;
    SSL_CTX *ctx;
    SSL *ssl;
    char req[1000];
    int req_len;
    char integer_string[32];
    //converts int time to a char arr
    sprintf(integer_string, "%d", time); 
    
    char hostname[256] = "odin.cs.csub.edu";
    char pagename[256] = "/~ahernandez2/3350/game/highscores.php?newtime=*";
    strcat(pagename, integer_string);
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
        //BIO_printf(outbio, "%s: Cannot connect to host %s [%s] on port %d.\n", 
        //argv[0], hostname, inet_ntoa(addr.sin_addr), port);
    }
    //Connect using the SSL certificate.
    ssl = SSL_new(ctx); 
    SSL_set_fd(ssl, sd);
    SSL_connect(ssl);
    //
    //Show the certificate data
    //	show_cert_data(ssl, outbio, hostname);	
    //A non-blocking socket will make the ssl_read() not block.
    set_to_non_blocking(sd);
    //Send the http request to the host server.
    sprintf(req, "GET /%s HTTP/1.1\r\nHost: %s\r\nUser-Agent: %s\r\n\r\n",  
            pagename, hostname, USERAGENT);    
    req_len = strlen(req);
    ret = SSL_write(ssl, req, req_len);
    if (ret <= 0)
        fprintf(stderr, "ERROR: SSL_write\n"); 
    fflush(stderr);
    //
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
        write(STDOUT_FILENO, buf, bytes);
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
    //	printf("\nn calls to ssl_read(): %i\n", nreads); fflush(stdout);
    //Cleanup.
    SSL_free(ssl);
    close(sd);
    SSL_CTX_free(ctx);
    }

