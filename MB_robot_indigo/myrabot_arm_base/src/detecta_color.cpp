  #include "ros/ros.h"
  #include "sensor_msgs/Image.h"
  #include "std_msgs/Bool.h"
  #include "image_transport/image_transport.h"

  #define li_red 255   //Límite inferior para canal rojo
  #define ls_red 255   //Límite superior para canal rojo
  #define li_green 255  //Límite inferior para canal verde
  #define ls_green 255   //Límite superior para canal verde
  #define li_blue 255   //Límite inferior para canal azul
  #define ls_blue 255   //Límite superior para canal azul
 
  sensor_msgs::Image region_imagen;   //Declaración de la variable global region
  
  image_transport::Publisher color_pub_; 
 
  void callback(const sensor_msgs::Image& ima)
  {    
        ros::NodeHandle n;
        
        ros::Publisher light_pub_=n.advertise<std_msgs::Bool>("light_status", 1);
 
        uint pix_r;
        uint pix_g;
        uint pix_b;
        uint ancho = ima.step;   
        uint tam = ima.data.size();
 
        int region[tam];
        
        for (uint i = 0; i < tam; i++)
        {
			region[i] = 0;
		}
        
        uint up;
        uint right;
        uint down;
        uint left;
        
        ::region_imagen = ima;
        
        uint cont_region = 299;
        
        std_msgs::Bool luz;
        
        luz.data = false;
 
        for (uint i = 0; i < tam; i = i + ancho)   //Filtro de regiones pequeñas o píxeles aislados
        {
                for (uint j = 0; j < ancho; j = j + 3)
                {
                        pix_r = ima.data[i + j];
                        pix_g = ima.data[i + j + 1];
                        pix_b = ima.data[i + j + 2];
 
                        if (pix_r >= li_red && pix_r <= ls_red && pix_g >= li_green && pix_g <= ls_green && pix_b >= li_blue && pix_b <= ls_blue)
                        {
                                up = 12;
                                if ((i/ancho) < 12)
                                {
                                        up = (i/ancho);
                                }
                                right = 12;
                                if (((ancho - j)/3) < 12)
                                {
                                        right = ((ancho - j)/3);
                                }
                                down = 12;
                                if (((tam/ancho) - (i/ancho)) < 12)
                                {
                                        down = ((tam/ancho) - (i/ancho));
                                }
                                left = 12;
                                if ((j/3) < 12)
                                {
                                        left = (j/3);
                                }
                               
                                uint contr = 0;
                                
                                for (uint k = i + j - (ancho*up) - (3*left); k <= (i + j + (ancho*down) + (3*right)); k = k + ancho)
                                {      
                                        for(uint l = 0; l < ((3*left) + (3*right) +3); l = l + 3)
                                        {
                                                uint pix_red = ima.data[k + l];
                                                uint pix_green = ima.data[k + l + 1];
                                                uint pix_blue = ima.data[k + l + 2];
 
                                        if (pix_red >= li_red && pix_red <= ls_red && pix_green >= li_green && pix_green <= ls_green && pix_blue >= li_blue && pix_blue <= ls_blue)
                                        {
                                                contr = contr + 1;
                                        }
                                        }
                                }
                                
                                if (contr >= ((25*25)*(1)))
                                {	
									up = 5;
									if ((i/ancho) < 5)
									{
											up = (i/ancho);
									}
									right = 5;
									if (((ancho - j)/3) < 5)
									{
											right = ((ancho - j)/3);
									}
									down = 5;
									if (((tam/ancho) - (i/ancho)) < 5)
									{
											down = ((tam/ancho) - (i/ancho));
									}
									left = 5;
									if ((j/3) < 5)
									{
											left = (j/3);
									}
									
									bool nueva_region = true;
									
									for (uint m = i + j - (ancho*up) - (3*left); m <= (i + j + (ancho*down) + (3*right)); m = m + ancho)
									{      
											for(uint n = 0; n < ((3*left) + (3*right) +3); n = n + 3)
											{
												pix_r = region[m + n];
												pix_g = region[m + n + 1];
												pix_b = region[m + n + 2];
	 
												if (pix_r > 299 && pix_g > 299 && pix_b > 299)
												{
													region[i + j] = pix_r;
													region[i + j + 1] = pix_g;
													region[i + j + 2] = pix_b;
													
													nueva_region = false;
												}
											}
									}
									
									
									if (nueva_region)
									{
										cont_region = cont_region + 1;
										
										//std::cout<<cont_region<<std::endl;
										
										region[i + j] = cont_region;
										region[i + j + 1] = cont_region;
										region[i + j + 2] = cont_region;
										
										//std::cout<<region[i + j]<<region[i + j + 1]<<region[i + j + 2]<<std::endl;
									}
									
									luz.data = true;
                                }
                                else   //Conversión a escala de grises regiones pequeñas o píxeles aislados
                                {
                                        if (pix_r > pix_g)
                                        {
                                                region[i + j] = pix_r;
                                                region[i + j + 1] = pix_r;
                                                region[i + j + 2] = pix_r;
                                        }
                                        else if (pix_g > pix_b)
                                        {
                                                region[i + j] = pix_g;
                                                region[i + j + 1] = pix_g;
                                                region[i + j + 2] = pix_g;
                                        }
                                        else
                                        {
                                                region[i + j] = pix_b;
                                                region[i + j + 1] = pix_b;
                                                region[i + j + 2] = pix_b;
                                        }
                                }
                        }
                        else   //Conversión a escala de grises resto píxeles
                        {
                                if (pix_r > pix_g)
                                {
                                        region[i + j] = pix_r;
                                        region[i + j + 1] = pix_r;
                                        region[i + j + 2] = pix_r;
                                }
                                else if (pix_g > pix_b)
                                {
                                        region[i + j] = pix_g;
                                        region[i + j + 1] = pix_g;
                                        region[i + j + 2] = pix_g;
                                }
                                else
                                {
                                        region[i + j] = pix_b;
                                        region[i + j + 1] = pix_b;
                                        region[i + j + 2] = pix_b;
                                }
                        }
                }
        }
        
        for (uint i = tam; i > 0; i = i - ancho)   //Filtro de regiones pequeñas o píxeles aislados
        {
                for (uint j = 3 ; j < ancho -3; j = j + 3)
                {
                        pix_r = ima.data[i - j];
                        pix_g = ima.data[i - j + 1];
                        pix_b = ima.data[i - j + 2];
 
                        if (pix_r >= li_red && pix_r <= ls_red && pix_g >= li_green && pix_g <= ls_green && pix_b >= li_blue && pix_b <= ls_blue)
                        {
                                up = 12;
                                if ((i/ancho) < 12)
                                {
                                        up = (i/ancho);
                                }
                                right = 12;
                                if (((ancho - j)/3) < 12)
                                {
                                        right = ((ancho - j)/3);
                                }
                                down = 12;
                                if (((tam/ancho) - (i/ancho)) < 12)
                                {
                                        down = ((tam/ancho) - (i/ancho));
                                }
                                left = 12;
                                if ((j/3) < 12)
                                {
                                        left = (j/3);
                                }
                               
                                uint contr = 0;
                                
                                for (uint k = i - j - (ancho*up) - (3*left); k <= (i - j + (ancho*down) + (3*right)); k = k + ancho)
                                {      
                                        for(uint l = 0; l < ((3*left) + (3*right) +3); l = l + 3)
                                        {
                                                uint pix_red = ima.data[k + l];
                                                uint pix_green = ima.data[k + l + 1];
                                                uint pix_blue = ima.data[k + l + 2];
 
                                        if (pix_red >= li_red && pix_red <= ls_red && pix_green >= li_green && pix_green <= ls_green && pix_blue >= li_blue && pix_blue <= ls_blue)
                                        {
                                                contr = contr + 1;
                                        }
                                        }
                                }
                                
                                if (contr >= ((25*25)*(1)))
                                {	
									up = 5;
									if ((i/ancho) < 5)
									{
											up = (i/ancho);
									}
									right = 5;
									if (((ancho - j)/3) < 5)
									{
											right = ((ancho - j)/3);
									}
									down = 5;
									if (((tam/ancho) - (i/ancho)) < 5)
									{
											down = ((tam/ancho) - (i/ancho));
									}
									left = 5;
									if ((j/3) < 5)
									{
											left = (j/3);
									}
									
									bool nueva_region = true;
									
									for (uint m = i - j - (ancho*up) - (3*left); m <= (i - j + (ancho*down) + (3*right)); m = m + ancho)
									{      
											for(uint n = 0; n < ((3*left) + (3*right) +3); n = n + 3)
											{
												pix_r = region[m + n];
												pix_g = region[m + n + 1];
												pix_b = region[m + n + 2];
	 
												if (pix_r > 299 && pix_g > 299 && pix_b > 299)
												{
													region[i - j] = pix_r;
													region[i - j + 1] = pix_g;
													region[i - j + 2] = pix_b;
													
													nueva_region = false;
												}
											}
									}
									
									
									if (nueva_region)
									{
										cont_region = cont_region + 1;
										
										std::cout<<"teta"<<cont_region<<std::endl;
										
										region[i - j] = cont_region;
										region[i - j + 1] = cont_region;
										region[i - j + 2] = cont_region;
										
										std::cout<<region[i - j]<<region[i - j + 1]<<region[i - j + 2]<<std::endl;
									}
									
									luz.data = true;
                                }
                                else   //Conversión a escala de grises regiones pequeñas o píxeles aislados
                                {
                                        if (pix_r > pix_g)
                                        {
                                                region[i - j] = pix_r;
                                                region[i - j + 1] = pix_r;
                                                region[i - j + 2] = pix_r;
                                        }
                                        else if (pix_g > pix_b)
                                        {
                                                region[i - j] = pix_g;
                                                region[i - j + 1] = pix_g;
                                                region[i - j + 2] = pix_g;
                                        }
                                        else
                                        {
                                                region[i - j] = pix_b;
                                                region[i - j + 1] = pix_b;
                                                region[i - j + 2] = pix_b;
                                        }
                                }
                        }
                        else   //Conversión a escala de grises resto píxeles
                        {
                                if (pix_r > pix_g)
                                {
                                        region[i - j] = pix_r;
                                        region[i - j + 1] = pix_r;
                                        region[i - j + 2] = pix_r;
                                }
                                else if (pix_g > pix_b)
                                {
                                        region[i - j] = pix_g;
                                        region[i - j + 1] = pix_g;
                                        region[i - j + 2] = pix_g;
                                }
                                else
                                {
                                        region[i - j] = pix_b;
                                        region[i - j + 1] = pix_b;
                                        region[i - j + 2] = pix_b;
                                }
                        }
                }
        }
        
        std::cout<<cont_region<<std::endl;
        std::cout<<"casco"<<std::endl;
        
        for (uint i = 0; i < tam; i = i + ancho)
        {
			for (uint j = 0; j < ancho; j = j + 3)
			{
				pix_r = region[i + j];
				pix_g = region[i + j + 1];
				pix_b = region[i + j + 2];

				if (pix_r == 300 && pix_g == 300 && pix_b == 300)
				{
					region[i + j] = 255;
					region[i + j + 1] = 0;
					region[i + j + 2] = 0;
				}
				else if (pix_r == 301 && pix_g == 301 && pix_b == 301)
				{
					region[i + j] = 0;
					region[i + j + 1] = 255;
					region[i + j + 2] = 0;
				}
				else if (pix_r > 301 && pix_g > 301 && pix_b > 301)
				{
					region[i + j] = 0;
					region[i + j + 1] = 0;
					region[i + j + 2] = 255;
				}
			}		
		}
        
        for (uint i = 0; i < tam; i++)
        {
			::region_imagen.data[i] = region[i];
		}
        
        light_pub_.publish(luz);
        
        color_pub_.publish(::region_imagen);
  }
 
  int main(int argc, char **argv)
  {
 
        ros::init(argc, argv, "sigue_rojo");
 
        ros::NodeHandle n;

        image_transport::ImageTransport it(n);
  
        color_pub_= it.advertise("xtion/light", 1);  
        
        ros::Publisher light_pub_=n.advertise<std_msgs::Bool>("light_status", 1);
 
        ros::Subscriber image_sub_= n.subscribe("xtion/rgb/image_color", 1, callback);
 
        ros::spin();
 
    return 0;
  }
