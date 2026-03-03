#ifndef RAWDATASET_H
#define RAWDATASET_H

//class QDialog;
//class QFileDialog;
//#include "vtkStructuredPoints.h"  
#include "qstring.h"
#include "qfile.h"
#include "qimage.h"
#include "qmessagebox.h"
#include "qtextstream.h"
#include "qmessagebox.h"
#include "qpushbutton.h"
#include "qobject.h"
#include <omp.h>
//class ByteMaskCollection;
//#include "ByteMaskCollection.h"

class RawDataSet
{

public:

	RawDataSet(int _n[3], double _scale[3],double _origin[3], double gantry = 0, bool Allocate = true);
	RawDataSet( int N1, int N2, int N3, double dx=1, double dy=1, double dz=1,double gantry=0,bool Allocate=true);
	RawDataSet();
	RawDataSet(const RawDataSet&);
	~RawDataSet( void );

	void SetGroupName(QString name) { groupName = name; };
	QString GetGroupName() { return groupName; };

	void	FindMinMaxValue();
	void	FindCircularMinMax(int *minMax);
	bool	SaveRGBDataAs3DataSets(const QString &ffilename);
	bool	SaveRGBRawDataSet( const QString &filename );
	bool	SaveRawDataSet( const QString &filename ,int Endian=2);
	bool	SaveRawDataSet(	const QString &filename	,int Endian,unsigned short *tQ);
	bool	SaveRawDataSet(	const QString &filename	,int Endian,unsigned char *tQ);
	bool	LoadRawDataSet( const QString &filename ,int _Nx=0,int _Ny=0, int _Nz=0 );
	bool	LoadRawDataSet(int n[3], double s[3], int depth, const QString& filename, int maxVal=0, int skip = 0, double* th = NULL,bool logData=false);

	bool	LoadRawDataSet2(const QString &filename);
	bool	SaveRawDataSet2(const QString &filename);

	bool	SavePngSlices(	const QString &filename);

	void	CalculateHistogram(void);
	void	SetRawDataSetName(const QString &name);
	QString GetRawDataSetName(){return RawDataSetName;};

	void	ConvertFromSignedData(bool clear=false,int minValue=0);

	bool	CompareWithOtherDataSet(RawDataSet *);
	void	Copy(const RawDataSet &Data,bool copydata);

	void	GetVoxelXYZPos(int pos, int p[3]);

	void	GetVoxelRealPos(int pos, double p[3]);

	inline int GetVoxelPos(int x, int y, int z ){	return z*SliceSize + y*Nx +	x;};
	inline unsigned short	GetVoxelValue( int x, int y, int z ){	return *(qArray	+ z*SliceSize +	y*Nx + x);};
	inline unsigned short  GetVoxelValueFromZeroValue( int x, int y, int z ){return *(qArray	+ z*SliceSize +	y*Nx + x)-FirstHistValue;};
	void	SetVoxelValue( int x, int y, int z, unsigned short value );
	
	void	ZeroQ(void);
	void	TestEndian(void);
	void	SwapData();

	inline unsigned short *GetDataArray(){return qArray;};
	void SetDataArray(unsigned short *_Q,bool clear=true)
	{
		if (clear==true) {if (qArray!=NULL) delete []qArray;};
		qArray=_Q;
	}

	void MakeCopyOfDataArray(unsigned short *_Q)
	{
		if (_Q!=NULL)
		{
			if (qArray==NULL) qArray=new unsigned short[VolumeSize];
			memcpy(qArray,_Q, VolumeSize*sizeof(unsigned short));
		}
	
	}

	//int dicomLeftThreshold, dicomRightThreshold;
	 double  dicomRescaleValue;
	 double dicomMinValue;
	 double dicomWindowWidthValue;
	 double dicomLevelValue;
	 int nrOfThreads;
	void					initRGBArrays();

	inline unsigned short	*GetRChannelArray(){return RChannelArray;};
	inline unsigned short	*GetGChannelArray(){return GChannelArray;};
	inline unsigned short	*GetBChannelArray(){return BChannelArray;};
	inline unsigned char	*GetBChannelUCHARArray(){return BChannelUCHARArray;};
	inline unsigned char	*GetGChannelUCHARArray(){return GChannelUCHARArray;};
	inline double			GetXscale(){return Xscale;};
	inline double			GetYscale(){return Yscale;};
	inline double			GetZscale(){return Zscale;};
	inline int				GetNx(){return Nx;};
	inline int				GetNy(){return Ny;};
	inline int				GetNz(){return Nz;};
	inline double			GetGantryAngle(){return GantryAngle;};
	inline int				GetSliceSize(){return SliceSize;};
	inline int				GetVolumeSize(){return VolumeSize;};
	inline int				GetMinValue(){return MinValue;};
	inline int				GetMaxValue(){return MaxValue;};
	
	void					GetN(int *n){n[0]=Nx;n[1]=Ny;n[2]=Nz;};
	void					GetScale(double *scale){scale[0]=Xscale;scale[1]=Yscale;scale[2]=Zscale;};
	void					GetPoint(int *p,int pos){p[2]=(int) (pos/SliceSize);	p[1]=(int) ((pos-p[2]*SliceSize)/Nx);	p[0]=(pos-p[2]*SliceSize)-Nx*p[1];};

	void			SetMaxValue(int a){ MaxValue=a;};//bad!

	inline double			GetMeanValue(){return MeanValue;};
	inline double			GetStandardDeviation(){return StandardDeviation;};
	inline bool				GetFileEndian(){return FileEndian;};
	inline bool				GetMachineEndian(){return MachineEndian;};

	inline int				*GetHistogramData(){return HistogramData;};
	inline int				GetHistMin(){return HistMin;};
	inline int				GetHistMax(){return  HistMax;};
	inline int				GetFirstHistValue(){return 	FirstHistValue;};
	inline int				GetLastHistValue(){return  LastHistValue;};

	void					SetOrigin(double *pos){origin[0]=pos[0];origin[1]=pos[1];origin[2]=pos[2];};
	void					GetOrigin(double *pos){pos[0]=origin[0];pos[1]=origin[1];pos[2]=origin[2];};



	void			SetFirstHistValue(int a){FirstHistValue=a;};
	void			SetLastHistValue(int a){LastHistValue=a;};

	inline bool    		GetHistogramCalculated(){return HistogramCalculated;};
	inline bool    		GetRGBExists(){return RGBExists;};
	inline int				GetMode(){return Mode;};

	void	SetRawDataSetPath(const QString path){ RawDataSetPath=path;};
	QString	GetRawDataSetPath(){return RawDataSetPath;};

	void					SetScale(double scale[3]){Xscale=scale[0];Yscale=scale[1];Zscale=scale[2];};
	void					SetScale(double xs,double ys,double zs){Xscale=xs;Yscale=ys;Zscale=zs;};

private:
	void ini(int N1, int N2, int N3, double dx = 1, double dy = 1, double dz = 1, double gantry = 0, bool Allocate = true);
	QString groupName;
	double origin[3];

	void			swap(unsigned short *a);
	unsigned short	swap(unsigned short a);


	QString				RawDataSetName;
	QString				RawDataSetPath;


	int					*HistogramData;
	int					HistMin,HistMax;
	int					FirstHistValue,LastHistValue;

	bool Init;
	bool HistogramCalculated;


	unsigned short	*qArray;          // pointer to the data array  
	unsigned short	*RChannelArray;
	unsigned short	*GChannelArray;
	unsigned short	*BChannelArray;
	unsigned char	*BChannelUCHARArray;
	unsigned char	*GChannelUCHARArray;
	int		Mode;





	bool	RGBExists;
	int		Nx, Ny, Nz;             // resolution of the dataset  
	double	Xscale,Yscale,Zscale;       // voxel sizes
	double	GantryAngle;
	int		SliceSize, VolumeSize;
	int		VolumeCenter[3];
	int		MinValue, MaxValue;
	double	MeanValue, StandardDeviation;
	bool	FileEndian,MachineEndian;

};




#endif 
