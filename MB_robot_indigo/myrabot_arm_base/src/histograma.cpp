  #include "ros/ros.h"
  #include "sensor_msgs/Image.h"
  #include "std_msgs/UInt8MultiArray.h"
 
  void callback(const sensor_msgs::Image& ima)
  {    
	ros::NodeHandle n;

	ros::Publisher histo_pub_= n.advertise<std_msgs::UInt8MultiArray>("histogram", 1);	
	
	uint pix_r;
	uint pix_g;
	uint pix_b;
	uint ancho = ima.step;   
	uint tam = ima.data.size();
	
	std_msgs::UInt8MultiArray histograma;
	
	/**histograma.layout.dim.push_back(std_msgs::MultiArrayDimension());
	histograma.layout.dim[0].label = "histogram";
	histograma.layout.dim[0].size = 1;
	histograma.layout.dim[0].stride = tam/3;
	
	histograma.data.clear();**/
	
	for (uint i = 0; i < 256; i++)
	{
		histograma.data.push_back(0);
	}
	
	std::cout<<histograma.data.size()<<std::endl;

	for (uint i = 0; i < tam; i = i + ancho)
	{
		for (uint j = 0; j < ancho; j = j + 3)
		{
			pix_r = ima.data[i + j];
			pix_g = ima.data[i + j + 1];
			pix_b = ima.data[i + j + 2];
			
			if (pix_r > pix_g && pix_r > pix_b)
			{
				histograma.data[pix_r] = histograma.data[pix_r] + 1;
			}
			else if (pix_g > pix_b)
			{
				histograma.data[pix_g] = histograma.data[pix_g] + 1;
			}
			else
			{
				histograma.data[pix_b] = histograma.data[pix_b] + 1;
			}
		}
	}
	
	histo_pub_.publish(histograma);
  }
 
  int main(int argc, char **argv)
  {
 
        ros::init(argc, argv, "histograma");
 
        ros::NodeHandle n;

		ros::Publisher histo_pub_= n.advertise<std_msgs::UInt8MultiArray>("histogram", 1);
 
        ros::Subscriber image_sub_= n.subscribe("xtion/rgb/image_raw", 1, callback);
 
        ros::spin();
 
    return 0;
  }
