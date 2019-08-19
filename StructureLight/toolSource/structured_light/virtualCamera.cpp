
#include "virtualCamera.h"
#include "utilities.h"



VirtualCamera::VirtualCamera(void)
{
	distortion=NULL;
	rotationMatrix=NULL;
	translationVector=NULL;
	cameraMatrix=NULL;
	projectionMatrix=NULL;
	fc.x=0;
	fc.y=0;
	cc.x=0;
	cc.y=0;
}


VirtualCamera::~VirtualCamera(void)
{

}

void VirtualCamera::loadDistortion(std::string path)
{
	loadMatrix(distortion,5,1,path);
}

void VirtualCamera::loadCameraMatrix(std::string path)
{
	cv::Mat camMatrix;
	loadMatrix(camMatrix,3,3,path);

	cc.x = Utilities::matGet2D(camMatrix,2,0);
	cc.y = Utilities::matGet2D(camMatrix,2,1);
	
	fc.x = Utilities::matGet2D(camMatrix,0,0);
	fc.y = Utilities::matGet2D(camMatrix,1,1);

	cameraMatrix = camMatrix;
}

void VirtualCamera::loadCalibrationError(std::string path)
{
    loadMatrix(calibrationError,2,1,path);
}

//推断此函数应该是求homograyhy()，projectionMatrix是3*4的，而homograyhy是3*3，而在matGet2D(translationVector,0,i)存在问题
void VirtualCamera::computeProjectionMatrix()
{
	projectionMatrix = cv::Mat(3, 4, CV_32F);

	for(int i=0; i<3; i++)
		for(int j=0; j<3; j++)
			Utilities::matSet2D(projectionMatrix,i,j,Utilities::matGet2D(rotationMatrix,i,j));

	for(int i=0; i<3; i++)
        Utilities::matSet2D(projectionMatrix,3,i,Utilities::matGet2D(translationVector,0,i));

	projectionMatrix = cameraMatrix*projectionMatrix;
}


void VirtualCamera::computeHomographyMatrix()
{
    homographyMatrix = cv::Mat(3, 3, CV_32F);

    for(int i=0; i<2; i++)
        for(int j=0; j<3; j++)
            Utilities::matSet2D(homographyMatrix,i,j,Utilities::matGet2D(rotationMatrix,i,j));

    for(int i=0; i<3; i++)
    {
        Utilities::matSet2D(homographyMatrix,2,i,Utilities::matGet2D(translationVector,0,i));
    }

    homographyMatrix = cameraMatrix*homographyMatrix;

}


void VirtualCamera::loadRotationMatrix(std::string path)
{
	loadMatrix(rotationMatrix,3,3,path);
}

void VirtualCamera::loadTranslationVector(std::string path)
{
	loadMatrix(translationVector,3,1,path);
}

int VirtualCamera::loadMatrix(cv::Mat &matrix,int rows,int cols ,std::string file)
{

	std:: ifstream in1; 
	in1.open(file.c_str());
	
    if(!in1)
	{
		std::cout<<"Error loading file "<<file.c_str()<<"\n";
		return -1;

	}

	if(!matrix.empty())
		matrix.release();

    matrix=cv::Mat(rows, cols, CV_32F);

	for(int i=0; i<rows; i++)
	{
		for(int j=0; j<cols; j++)
		{
            float val;
			in1>>val;

			Utilities::matSet2D(matrix,j,i,val);

		}
	}
	return 1;
}
