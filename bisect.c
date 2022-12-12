


#include  "FPToolkit.c"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
   int    swidth, sheight ;
   double  PI = 3.1415926535897932;
   double p[2], q[2], k[2];

 
   swidth = 600 ;  sheight = 600 ;
   G_init_graphics (swidth,sheight) ;
   

   G_rgb (0.3, 0.3, 0.3) ; // dark gray
   G_clear () ;

   

   
   // draw a line
   //G_rgb (0.0, 1.0, 0.0) ; // green
   //G_line (0,0, swidth-1, sheight-1) ;

  

   
   // circles   
   //G_rgb (1.0, 0.5, 0.0) ; // orange
   //G_circle (100, 300, 75) ;
   //G_fill_circle (370, 200, 50) ;

   



   //===============================================

   
   G_rgb(1,0,0) ;
   
   G_wait_click(p) ;
   G_fill_circle(p[0],p[1],2) ;

   G_wait_click(q) ;
   G_fill_circle(q[0],q[1],2) ;

   G_line(p[0],p[1],q[0],q[1]);

     
   G_rgb(0,1,0);
   G_wait_click(k);
   G_fill_circle(k[0],k[1],2);
   G_line(0, k[1], swidth, k[1]);

   double x = (k[1] - p[1])*(q[0] - p[0]) / (q[1] - p[1]) + p[0];

   

   if((p[1] <=  k[1] && k[1]  <=  q[1])||(p[1] >=  k[1] && k[1] >= q[1])){
     G_rgb(1,0,0);
     G_fill_circle(x, k[1],2);
   }
   
   
   int key ;   
   key =  G_wait_key() ; // pause so user can see results

   //   G_save_image_to_file("demo.xwd") ;
   G_save_to_bmp_file("demo.bmp") ;
}

