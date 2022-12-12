
/*
    FPToolkit.c : A simple set of graphical tools.
    FPToolkitDemo.c 
    Copyright (C) 2018  Ely

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License (version 3)
    as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/





/*

This code assumes an underlying X11 environment.

       Most freshly installed unbuntu environments do not have
       the X11 developr stuff they'll need to do graphics.
       To download X11 developer stuff, connect to the internet and 
       issue the following two commands.  Each will ask for your password
       and each will take a few minutes.  At some point it might even
       look like nothing is happening....be patient :
       
sudo  apt-get  install  libx11-dev     

sudo  apt-get  install  xorg-dev

*/




/*
If this file and the file, FPToolkit.c, are in the
same directory that you are trying to compile in,  
do the following :

cc  FPToolkitDemoB.c   -lm  -lX11

note : the 'X' in -X11 is capitalized
*/


#include  "FPToolkit.c"


int main()
{
   int    swidth, sheight ;
   double  PI = 3.1415926535897932;


   
   // must do this before you do 'almost' any other graphical tasks 
   swidth = 600 ;  sheight = 600 ;
   G_init_graphics (swidth,sheight) ;  // interactive graphics

   
   // clear the screen in a given color
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

   double p[2], q[2], r, theta, i;
   double spokes = 16;

   G_rgb(1,0,0) ;
   
   G_wait_click(p) ;
   G_fill_circle(p[0],p[1],2) ;

   G_wait_click(q) ;
   G_fill_circle(q[0],q[1],2) ;   

   G_rgb(0,1,0.5) ;
   r  = sqrt(pow(p[0] - q[0],2) + pow(p[1] - q[1],2));
   G_circle(p[0],p[1], r) ;


   theta = 0;
   for (i = 0; i < spokes; i++){

     theta = 2 * PI/spokes * i;
     double radialx = p[0] + r * cos(theta);
     double radialy = p[1] + r * sin(theta);
     G_line(p[0],p[1], radialx, radialy);

   }
   int key ;   
   key =  G_wait_key() ; // pause so user can see results

   //   G_save_image_to_file("demo.xwd") ;
   G_save_to_bmp_file("demo.bmp") ;
}

