#pragma once
#include<math.h>
#include<algorithm>
#include<vector>
#include<list>
#include<stdio.h>
using namespace std;
#include "cv.h"
using namespace cv;

typedef struct{
 int x,y,z;
}pointsGet;

typedef struct{
   
    //double threshHold;
    pointsGet Center;
    double xlenght,ylenght,zlenght;
    int members;
    list<long> lists;

}cube;
bool compareOut(CvPoint ,CvPoint );
class Voxel{

private:
    cube **voxelCube;
    int dx,dy,dz;
    double shiftXYZ[3];
	CvPoint maxMin[2][3];
	vector<CvPoint> *pointCloud;
    long cloudDim;
    int spaceDim;
    long xsize,ysize,zsize;
    double threshHold;
    //typedef vector<double> subCon;
    typedef vector<CvPoint> listPoints;
    
public:
	listPoints listofPoints;
	Voxel(vector<CvPoint> *pointData,int dx,int dy,int dz = 0,long lenght = 1,int spaceDim = 1,double th = 5){
        
        this->cloudDim = lenght;
        this->spaceDim = spaceDim;
		this->pointCloud = pointData;
		maxMin[0][0].x = maxMin[0][0].y = 0;
		maxMin[0][1].x = maxMin[0][1].y = 0;
		maxMin[0][2].x = maxMin[0][2].y = 0;
		maxMin[1][0].x = maxMin[1][0].y = 64000;
		maxMin[1][1].x = maxMin[1][1].y = 64000;
		maxMin[1][2].x = maxMin[1][1].y = 64000;
        this->dx = dx;
        this->dy = dy;
		this->dz = 0;
        this->threshHold=th;
        shiftXYZ[0]=0;
        shiftXYZ[1]=0;
        shiftXYZ[2]=0;
		std::sort(this->pointCloud->begin(),this->pointCloud->end(),compareOut);
    }
	
    void voxelInitiator(double **);
    void maxminFinder();
    void gridCreation();
    void histogramGeneration();
    void voxelFiltering();
   
};

bool compareOut( CvPoint p1,  CvPoint p2){
	 return p1.x == p2.x ? p1.y < p2.y : p1.x < p2.x;
}
void Voxel::gridCreation(){

	xsize=(((maxMin[0][0].x + shiftXYZ[0])-(maxMin[1][0].x + shiftXYZ[0]))/dx) + 1;
	ysize=(((maxMin[0][1].y + shiftXYZ[1])-(maxMin[1][1].y + shiftXYZ[1]))/dy) + 1;
    //zsize=(((maxMin[0][2]+shiftXYZ[2])-(maxMin[1][2]+shiftXYZ[2]))/dz)+1;
   // printf("Min points %d %d %d \n",xsize,ysize,zsize);
	printf("xsize = %d ysize = %d\n",xsize,ysize);
    voxelCube = new cube*[xsize+1];
    for(long i=0;i <= xsize; i++)
        voxelCube[i] = new cube[ysize+1];
    //for(long i=0;i<=xsize;i++)
      //  for(long j=0;j<=ysize;j++)
        //    voxelCube[i][j] = new cube[zsize+1];
    for(long i = 0; i <= xsize; i++)
		for(long j = 0; j <= ysize; j++){
           // for(long k=0;k<=zsize;k++){
                voxelCube[i][j].members = 0;
                voxelCube[i][j].Center.x = 0;
                voxelCube[i][j].Center.y = 0;
                //voxelCube[i][j][k].Center.z=0;

        }
    return;
}
void Voxel::histogramGeneration(){

    long tempX,tempY,tempZ;
   // subCon vec;
    //printf("Min points %d %d %d \n",xsize,ysize,zsize);
    for(long i=0;i<this->cloudDim;i++)
    {
		tempX=(long)(fabs((pointCloud->at(i).x + shiftXYZ[0] - maxMin[1][0].x))/dx);
		tempY=(long)(fabs((pointCloud->at(i).y + shiftXYZ[1] - maxMin[1][1].y ))/dy);
       // tempZ=(long)(fabs((pointCloud[2][i]+shiftXYZ[2]))/dz);
        //printf("Min points %d %d %d \n",tempX,tempY,tempZ);
		voxelCube[tempX][tempY].Center.x += pointCloud->at(i).x;
		voxelCube[tempX][tempY].Center.y += pointCloud->at(i).y;
        //voxelCube[tempX][tempY].Center.z += pointCloud[2][i];
        voxelCube[tempX][tempY].members++;
        //vec.push_back(pointCloud[0][i]);
        //vec.push_back(pointCloud[1][i]);
        //vec.push_back(pointCloud[2][i]);
        //listofPoints.push_back(vec);
        //vec.clear();
        voxelCube[tempX][tempY].lists.push_back(i);

    }
}
void Voxel::voxelFiltering(){

    long i,j,k;
    //subCon Vec1;
    list<long>::iterator it;
 ///   printf(" voxel size=%d\n",voxelCube[i][j][k].members);
    for(i=0;i<=xsize;i++)
        for(j=0;j<=ysize;j++){
			if(voxelCube[i][j].members != 0)
				listofPoints.push_back(cvPoint(voxelCube[i][j].Center.x/voxelCube[i][j].members,voxelCube[i][j].Center.y/voxelCube[i][j].members));
        }
		pointCloud->clear();
}
void Voxel::maxminFinder(){

    
        for(long j=0;j<cloudDim;j++){

			if(pointCloud->at(j).x > maxMin[0][0].x)
				maxMin[0][0].x = pointCloud->at(j).x;
			if(pointCloud->at(j).x < maxMin[1][0].x)
				maxMin[1][0].x = pointCloud->at(j).x;
        }
		for(long j=0; j < cloudDim; j++){

			if(pointCloud->at(j).y > maxMin[0][1].y)
				maxMin[0][1].y = pointCloud->at(j).y;
			if(pointCloud->at(j).y < maxMin[1][1].y)
				maxMin[1][1].y = pointCloud->at(j).y;
        }

    for(int i = 0; i < spaceDim; i++)
    {
		//if(maxMin[1][i].x < 0)
          //  shiftXYZ[i] = maxMin[1][i]*(-1);
        //else
            shiftXYZ[i] = 0;
    }

    return;
}
