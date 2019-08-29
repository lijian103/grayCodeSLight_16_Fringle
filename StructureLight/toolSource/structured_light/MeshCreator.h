/**@file time_measurement.h
 * @brief contains definitions for the TimeMeasurement Class
 * @copyright lijian
 * data 2019/8/22
 */
#ifndef MESH_CREATOR_
#define MESH_CREATOR_

#include "PointCloudImage.h"
#include "utilities.h"

class MeshCreator
{

public:

	MeshCreator(PointCloudImage *in);
	~MeshCreator(void);
	void exportObjMesh(std::string path);
	void exportPlyMesh(std::string path, bool gridFormat);

private:

	int *pixelNum;
	PointCloudImage *cloud;
	int MeshCreator::access(int i,int j);
	int MeshCreator::access(int i,int j, int z);
	int w;
	int h;

};

#endif //_MESH_CREATOR_
