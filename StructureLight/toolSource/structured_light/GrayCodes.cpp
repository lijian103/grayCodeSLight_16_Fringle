/**@file time_measurement.h
 * @brief contains definitions for the GRAY_CODES Class
 * @copyright lijian
 * data 2019/8/23
 */

#include "GrayCodes.h"


GrayCodes::GrayCodes(int projW, int projH,  std::string path)
{
	for (int i=0; i<GRAY_MAX_NUM; i++)
	{
        grayCodes[i]=nullptr;

	}

	height=projH;
	width=projW;

	imgsLoaded=false;

	calNumOfImgs();

	currentImgNum=0;

    this->path = path;

}

GrayCodes::~GrayCodes()	
{
	if(imgsLoaded)
		unload();
}

int GrayCodes::getNumOfColBits()
{
	return numOfColImgs;
}

int GrayCodes::getNumOfRowBits()
{
	return numOfRowImgs;
}

void GrayCodes::calNumOfImgs()
{
	numOfColImgs = (int)ceil(log(double (width))/log(2.0));
	numOfRowImgs = (int)ceil(log(double (height))/log(2.0));

	numOfImgs= 2 + 2*numOfColImgs + 2*numOfRowImgs;
}

void GrayCodes::unload()
{

	for(int i=0; i < numOfImgs;i++ )
	{
		if(grayCodes[i])
		{      
            grayCodes[i]->release();
            delete  grayCodes[i];
            grayCodes[i]=nullptr;
		}
	}
	imgsLoaded=false;
}

cv::Mat* GrayCodes::getImg(int num)
{
	if(num<numOfImgs)
	{
		currentImgNum = num;
		return grayCodes[num];
	}
	else
        return nullptr;
}

cv::Mat*  GrayCodes::getNextImg()
{
	
	if(currentImgNum<numOfImgs)
	{
		currentImgNum++;
		return grayCodes[currentImgNum-1];
	}
	else
        return nullptr;
}

int GrayCodes::getNumOfImgs()
{
	return numOfImgs;
}


void GrayCodes::allocMemForImgs()
{

	for(int i=0; i<numOfImgs; i++)
	{
       grayCodes[i] =  new cv::Mat(height,width,CV_8UC1);
	}

	imgsLoaded=true;
}

void GrayCodes::generateGrays()
{
	if(!imgsLoaded)
	{
		allocMemForImgs();
	}

	//generate all white and all black images
	for (int j=0; j<width; j++)	
	{	
		for (int i=0;i<height;i++)	
        {
            int pixel_color = 255;
            Utilities::matSet2D(*grayCodes[0], j, i, pixel_color);


            pixel_color = 0;
            Utilities::matSet2D(*grayCodes[1], j, i, pixel_color);
		} 
	}


	int flag=0;

    for (int j = 0; j < width; j++)
	{
        int rem = 0, num = j, prevRem = j%2;
		
        for (int k = 0; k < numOfColImgs; k++)
		{
			
			num=num/2;
			rem=num%2;

            if ((rem == 0 && prevRem == 1) || (rem == 1 && prevRem == 0))
			{ 
				flag=1;
			}
			else 
			{
				flag= 0;
			}

            for (int i = 0;i < height;i++)
			{
                int pixel_color;
                pixel_color = flag*255;
                Utilities::matSet2D(*grayCodes[2*numOfColImgs-2*k], j, i, pixel_color);

                if(pixel_color>0)
                    pixel_color=0;
                else
                    pixel_color=255;

                Utilities::matSet2D(*grayCodes[2*numOfColImgs-2*k+1], j, i, pixel_color);
			}

			prevRem=rem;
		} 
		
	}

	for (int i=0;i<height;i++)	
	{
		int rem=0, num=i, prevRem=i%2;
		
		for (int k=0; k<numOfRowImgs; k++)	
		{

			num=num/2;
			rem=num%2;

			if ((rem==0 && prevRem==1) || (rem==1 && prevRem==0))
			{ 
				flag=1;
			}
			else 
			{
				flag= 0;
			}

			for (int j=0; j<width; j++)	
			{
                int  pixel_color;
                pixel_color = flag*255;
                Utilities::matSet2D(*grayCodes[2*numOfRowImgs-2*k+2*numOfColImgs], j, i, pixel_color);

                if(pixel_color>0)
                    pixel_color=0;
				else
                    pixel_color=255;

                Utilities::matSet2D(*grayCodes[2*numOfRowImgs-2*k+2*numOfColImgs+1], j, i, pixel_color);

			}

			prevRem=rem;
		} 
		
	}
}

void GrayCodes::save()
{
	for(int i = 0; i < numOfImgs; i++)
	{

        std::stringstream tempPath;
        if(i+1<10)
            tempPath<<"0";
		
        tempPath<< i+1 <<".bmp";

        std::string tempstrPath;
        tempPath>>tempstrPath;

        tempstrPath=path+tempstrPath;

        cv::Mat tempMat;
        std::vector<cv::Mat> channels(3);
        channels[0]=*grayCodes[i];
        channels[1]=*grayCodes[i];
        channels[2]=*grayCodes[i];

        cv::merge(channels,tempMat);
        cv::imwrite(tempstrPath.c_str(),tempMat);
	}
}

//convert a gray code sequence to a decimal number
int GrayCodes::grayToDec(std::vector<bool> gray)
{
	int dec=0;
	bool tmp = gray[0];
	
	if(tmp)
		dec+=(int) pow((float)2,int (gray.size() -1));

	for(int i=1; i< gray.size(); i++)
	{
		tmp=Utilities::XOR(tmp,gray[i]);

		if(tmp)
			dec+= (int) pow((float)2,int (gray.size()-i-1) );
	}

	return dec;
}


