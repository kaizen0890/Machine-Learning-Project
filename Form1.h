#pragma once
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/contrib/contrib.hpp"
#include <iostream>
#include <msclr/marshal_cppstd.h>
#include "math.h"

using namespace std;
using namespace cv;
using namespace std;
using namespace msclr::interop;
using namespace System::Runtime::InteropServices;

//----------------------------------------------------------------------------------------------------------

static int **roi_R_matrix=NULL,**roi_G_matrix=NULL,**roi_B_matrix=NULL; //ROI

static int **roi_matrix=NULL,**energy_matrix=NULL;

static int RoiH,RoiW; 

static int otsu_hist[256];

static int roi_cut=10;

static int InputH,InputW; 

static int **R_matrix=NULL,**G_matrix=NULL,**B_matrix=NULL,**Gray_matrix=NULL; //Original

void calculate_hist(Mat input, Mat output)
{
	// tach anh thanh tung kenh mau rieng biet

	vector<Mat> planes;
	split(input , planes);
	// Khai bao kich thuoc mang gia tri histogram  
	int histSize = 256;
	Mat input_b , input_g , input_r ;
	float range[] = {0 , 255};
	const float *Histrange = {range};
	// ham tinh toan histogram cua anh
	// gia tri sau khi tinh toan se dua vào mảng histogram (histsize, histRange)
	cv::calcHist(&planes[0], 1 , 0 , Mat() , input_b , 1 , &histSize , &Histrange , true , false);
	cv::calcHist(&planes[1], 1 , 0 , Mat() , input_g , 1 , &histSize , &Histrange , true , false);
	cv::calcHist(&planes[2], 1 , 0 , Mat() , input_r , 1 , &histSize , &Histrange , true , false);
	// tao anh voi kich thuoc hist_u va hist_h (gia tri co the dat tuy y) de hien thi histogram
	int hist_w = 128;
	int hist_h = 128;
	Mat histogram(hist_h , hist_w , CV_8UC3 , Scalar( 255 , 255, 255)); // (255,255,255) la anh nen mau trang
	// tinh toan gia tri tuong ung giua kich thuoc mang Histsize va anh hien thi histogram 

	int bin_w = cvRound( (double)(hist_w/histSize + 0.5));

	normalize(input_b , input_b, 0, histogram.rows, NORM_MINMAX, -1, Mat() );
	normalize(input_g , input_g, 0, histogram.rows, NORM_MINMAX, -1, Mat() );
	normalize(input_r , input_r, 0, histogram.rows, NORM_MINMAX, -1, Mat() );

	// duyet bieu do histogram de ve duong hien thi
	for (int i = 1; i < histSize; i++)
	{
		// ---------------------HIEN THI THEO GIA TRI DINH-----------------------
		line( histogram, cv::Point( bin_w*(i-1), hist_h - cvRound(input_b.at<float>(i-1)) ) ,
			cv::Point( bin_w*(i), hist_h - cvRound(input_b.at<float>(i)) ),
			Scalar( 255, 0, 0), 2, 8, 0 );//blue

		line( histogram, cv::Point( bin_w*(i-1), hist_h - cvRound(input_g.at<float>(i-1)) ) ,
			cv::Point( bin_w*(i), hist_h - cvRound(input_b.at<float>(i)) ),
			Scalar( 0, 255, 0), 2, 8, 0);//green

		line( histogram, cv::Point( bin_w*(i-1), hist_h - cvRound(input_r.at<float>(i-1)) ) ,
			cv::Point( bin_w*(i), hist_h - cvRound(input_b.at<float>(i)) ),
			Scalar( 0, 0, 255), 2, 8, 0 );//red

		// ----------------------HIEN THI THEO MANG GIA TRI------------------------

		//line( histogram, cv::Point( bin_w*(i-1), hist_h) ,
		//	 cv::Point( bin_w*(i), hist_h - cvRound(img_b.at<float>(i)) ),
		//                    Scalar( 255, 0, 0), 2, 8, 0 );  //blue

		//line( histogram, cv::Point( bin_w*(i-1), hist_h) ,
		//	 cv::Point( bin_w*(i), hist_h - cvRound(img_g.at<float>(i)) ),
		//                    Scalar( 0, 255, 0), 2, 8, 0);   //green

		//line( histogram, cv::Point( bin_w*(i-1), hist_h) ,
		//                    cv::Point( bin_w*(i), hist_h - cvRound(img_r.at<float>(i)) ),
		//                    Scalar( 0, 0, 255), 2, 8, 0 );  //red

	}	

	int rows_hist = (histogram.rows/4)*4;
	int cols_hist = (histogram.cols/4)*4;

	output = Mat(rows_hist, cols_hist, CV_8UC3);
	resize(histogram, output, output.size(), 0, 0, INTER_LINEAR);



}

//************************************************************************************************
//******************************************FUNCTIONS*************************************************
//****************************************************************************************************

// ------------------ Nhi phan hoa anh su dung nguong OTSU------------------
static int o_tsu_Algorithm(int input_hist[256],int input_h,int input_w)
{
	int t_Threshold=0,f_max=0;
	int N=input_h*input_w;

	for(int t=0;t<=255;t++)
	{
		float w0=0,w1=0,u0=0,u1=0,u=0;

		int f=0;
		for(int i=0;i<=t-1;i++)
		{
			w0=w0+((float)input_hist[i]/N);
			u=u+(((float)input_hist[i]/N)*i);

		}
		if(w0!=0)
			u0=u/w0;
		u=0;
		for(int i=t;i<=255;i++)
		{
			w1=w1+((float)input_hist[i]/N);
			u=u+(((float)input_hist[i]/N)*i);
		}
		if(w1!=0)
			u1=u/w1;
		f=w0*w1*(u0-u1)*(u0-u1);
		if(f>f_max)
		{
			f_max=f;
			t_Threshold=t;
		}
	}
	return t_Threshold;
}   


static void ea_process(int **input_matrix,int input_matrix_width,int input_matrix_height,
	bool inverse_flag,bool binarization_flag,int **output_matrix,int in_delta)
{
	int delta=in_delta;
	int mask_coef=((delta*2)+1)*((delta*2)+1);
	int **eng_mat;
	eng_mat=new int *[input_matrix_height];

	for(int i=0;i<input_matrix_height-1;i++)
	{
		eng_mat[i]=new int[input_matrix_width];
		for(int j=0;j<input_matrix_width-1;j++)
		{
			eng_mat[i][j]=0;

			if((i-delta<0 || j-delta<0) || (i+delta>=input_matrix_height || j+delta>=input_matrix_width) );
			else
			{
				int Value1=0;
				int Mean=0;

				for(int Ty=i-delta;Ty<=i+delta;Ty++)
				{
					for(int Tx=j-delta;Tx<=j+delta;Tx++)
					{

						Value1+=input_matrix[Ty][Tx];
					}
				}
				Mean=Value1/mask_coef;
				int Value2=0;

				for(int Ty=i-delta;Ty<=i+delta;Ty++)
				{
					for(int Tx=j-delta;Tx<=j+delta;Tx++)
					{
						Value2+=((input_matrix[Ty][Tx]-Mean)*(input_matrix[Ty][Tx]-Mean));
					}
				}
				eng_mat[i][j]=(Value2/mask_coef);


				if(eng_mat[i][j]>=255) eng_mat[i][j]=255;
				if(eng_mat[i][j]<=0)  eng_mat[i][j]=0;


			}
			if(inverse_flag==false)
			{
				if(binarization_flag==true)
				{
					if(eng_mat[i][j]>=255)
						output_matrix[i][j]=255;
					else
						output_matrix[i][j]=0;
				}
				else
					output_matrix[i][j]=eng_mat[i][j];
			}
			else if (inverse_flag==true)
			{
				eng_mat[i][j]=(255-eng_mat[i][j]);
				if(binarization_flag==true)
				{
					if(eng_mat[i][j]<=0)
						output_matrix[i][j]=0;
					else
						output_matrix[i][j]=255;
				}
				else
					output_matrix[i][j]=eng_mat[i][j];
			}
		}

	}
	for(int y=0;y<=input_matrix_height-1-(in_delta*2);y++)
	{
		delete [] eng_mat[y];
	}
	delete [] eng_mat;

}
//====================================================================================================
//static void Matrix2DDelete(int **DeleteMatrix,int bmpH)
//{
//	for(int y=0;y<=bmpH-1;y++)
//		delete [] DeleteMatrix[y];
//	delete [] DeleteMatrix;
//}
//=====================================================================================================









namespace Inspectionproject {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  bt_open;
	protected: 
	private: System::Windows::Forms::PictureBox^  pB_EAproces;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog;


	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->bt_open = (gcnew System::Windows::Forms::Button());
			this->pB_EAproces = (gcnew System::Windows::Forms::PictureBox());
			this->openFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_EAproces))->BeginInit();
			this->SuspendLayout();
			// 
			// bt_open
			// 
			this->bt_open->Location = System::Drawing::Point(96, 55);
			this->bt_open->Name = L"bt_open";
			this->bt_open->Size = System::Drawing::Size(102, 56);
			this->bt_open->TabIndex = 0;
			this->bt_open->Text = L"OPEN";
			this->bt_open->UseVisualStyleBackColor = true;
			this->bt_open->Click += gcnew System::EventHandler(this, &Form1::bt_open_Click);
			// 
			// pB_EAproces
			// 
			this->pB_EAproces->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pB_EAproces->Location = System::Drawing::Point(430, 55);
			this->pB_EAproces->Name = L"pB_EAproces";
			this->pB_EAproces->Size = System::Drawing::Size(344, 307);
			this->pB_EAproces->TabIndex = 1;
			this->pB_EAproces->TabStop = false;
			this->pB_EAproces->Click += gcnew System::EventHandler(this, &Form1::pB_EAproces_Click);
			// 
			// openFileDialog
			// 
			this->openFileDialog->FileName = L"openFileDialog";
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(926, 434);
			this->Controls->Add(this->pB_EAproces);
			this->Controls->Add(this->bt_open);
			this->Name = L"Form1";
			this->Text = L"Form1";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_EAproces))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void bt_open_Click(System::Object^  sender, System::EventArgs^  e) {


				 openFileDialog->Filter = "Image Files|*.bmp; *.jpg; *.png; *.pgm|All Files (*.*)|*.*||"; 
				 openFileDialog->Title = "Select a image file"; 
				 openFileDialog->InitialDirectory = "";
				 openFileDialog-> Multiselect = "True"; 

				 int number_of_files;
				 array<System::String^>^ filename_src; // array of filenames
				 if ( openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK )  
				 {
					 filename_src = openFileDialog->FileNames;
					 number_of_files = openFileDialog->FileNames->Length;
				 }
				 else
					 return;
				 int step = 0;
				 while (step < number_of_files)
				 {
					 System::String^ OpenFileName;
					 OpenFileName = filename_src[step];//duong dan day du, kieu du lieu System::String^

					 OpenFileName = OpenFileName->Replace("\\","\\\\");// opencv pathfile is C:\\folder\\filename
					 //Bitmap^ OpenBmp = gcnew Bitmap(OpenFileName);
					 //pB_orig->Image = OpenBmp;
					 //pB_orig->Refresh();

					 char* imgname;
					 imgname = (char*)Marshal::StringToHGlobalAnsi(OpenFileName).ToPointer();

					 Mat img = imread(imgname,CV_LOAD_IMAGE_COLOR);

					 // Ket thuc mo file anh

					 Mat process_image = img.clone();
					 int col_s = img.cols;
					 int row_s = img.rows;


					 InputH = img.cols;
					 InputW = img.rows;

					 R_matrix=new int *[InputH];    // create array pointer 
					 G_matrix=new int *[InputH];
					 B_matrix=new int *[InputH];
					 Gray_matrix=new int *[InputH];


					 for(int y=0;y<=InputH-1;y++)
					 {
						 //------------------Read the information within process_image-------------------

						 R_matrix[y]=new int [InputW];
						 G_matrix[y]=new int [InputW];
						 B_matrix[y]=new int [InputW];
						 Gray_matrix[y]=new int [InputW];
						 for(int x=0;x<=InputW-1;x++)
						 {
							 R_matrix[y][x] = process_image.at<Vec3b>(cv::Point(y, x))[2];
							 G_matrix[y][x] = process_image.at<Vec3b>(cv::Point(y, x))[1];
							 B_matrix[y][x] = process_image.at<Vec3b>(cv::Point(y, x))[0];
							 Gray_matrix[y][x]= int(double(process_image.at<Vec3b>(cv::Point(y, x))[2]+process_image.at<Vec3b>(cv::Point(y, x))[1]+process_image.at<Vec3b>(cv::Point(y, x))[0])/3.0);
							 //------------------Computing a histogram --------------------------
							 otsu_hist[Gray_matrix[y][x]]++;   
						 }
					 }


					 //-----------------------------Use otsu_hist compute otsu Threshold----------------
					 int otsu_t=o_tsu_Algorithm(otsu_hist,InputH,InputW);
					 //-----------------------------The gray-scale map using the threshold for binarization-----------------
					 cv::Mat otsu_image = cv::Mat(InputW,InputH, CV_8UC3);

					 for(int y=0; y<=InputH-1;y++)
					 {
						 for(int x=0;x<=InputW-1;x++)
						 {
							 int r;
							 if(Gray_matrix[y][x]>= otsu_t) {r=255;} else {r=0;}

							 otsu_image.at<Vec3b>(cv::Point(y, x))[2] = r;
							 otsu_image.at<Vec3b>(cv::Point(y, x))[1] = r;
							 otsu_image.at<Vec3b>(cv::Point(y, x))[0] = r;
						 }
					 }

					 

					 //------------------------------Hien thi anh nhi phan bang OTSU----------------------------
					 Mat otsu_image_s = Mat(col_s, row_s, otsu_image.type());
					 resize(otsu_image, otsu_image_s, otsu_image_s.size(), 0, 0, INTER_LINEAR);

					 

					 RoiH = img.cols;
					 RoiW = img.rows;
					
					 Mat image = img.clone();
					 R_matrix=new int *[RoiH];
					 G_matrix=new int *[RoiH];
					 B_matrix=new int *[RoiH];
					 
					 for(int y=0;y<=RoiH-1;y++)
					 {
						 //Byte *TempRow=(Byte *)Form1->roi_image->ScanLine[y];
						 R_matrix[y]=new int [RoiW];
						 G_matrix[y]=new int [RoiW];
						 B_matrix[y]=new int [RoiW];

						 for(int x=0;x<=RoiW-1;x++)
						 {
							 R_matrix[y][x]=image.at<Vec3b>(cv::Point(y, x))[2];
							 G_matrix[y][x]=image.at<Vec3b>(cv::Point(y, x))[1];
							 B_matrix[y][x]=image.at<Vec3b>(cv::Point(y, x))[0];
						 }
					 }

					

					 Mat energy_image = Mat(RoiW,RoiH, img.type());
					 energy_matrix=new int *[RoiH];
					 for(int y=0;y<=RoiH-1;y++)
					 {
						 energy_matrix[y]=new int [RoiW];
					 }
					 //--------------------Localized energy analysis done for roi_G_matrix--------------------
					 ea_process(G_matrix,RoiW,RoiH,false,false,energy_matrix,1); 

					 int EnergyThreshold = 50;

					 
					 for(int y=0;y<=RoiH-1;y++)
					 {

						 for(int x=0;x<=RoiW-1;x++)
						 {
							 if (energy_matrix[y][x] < EnergyThreshold)
							 {
								 energy_matrix[y][x] = 0;
							 }
							 else 
								 energy_matrix[y][x] = 255;
						 }
					 }

					 for(int y=0;y<=RoiH-1;y++)
					 {

						 for(int x=0;x<=RoiW-1;x++)
						 {
							 energy_image.at<Vec3b>(cv::Point(y, x)) = energy_matrix[y][x];
						 }
					 }
					 
					 imshow("",energy_image);


					 //--------------------------------- tinh histogram cua anh ----------------------------------

					 int *vertical_value=new int [InputW];
					 int *horizon_value=new int [InputH];
					 //-----------------------------The initial value of zero -------------------------------
					 for(int i=0;i<=InputW-1;i++)
					 {
						 vertical_value[i]=0;
					 }
					 for(int i=0;i<=InputH-1;i++)
					 {
						 horizon_value[i]=0;
					 }
					 //-----------------------------Projected amount decided otsu_image---------------------
					 for(int y=0;y<=InputH-1;y++)
					 {
						 for(int x=0;x<=InputW-1;x++)
						 {
							 //-------------otsu_image The information was greater than the threshold Projection Projection-----
							 if(energy_matrix[y][x]>EnergyThreshold)
							 {
								 vertical_value[x]++;
								 horizon_value[y]++;
							 }
						 }
					 }
					 //-----------------------------Find maximum vertical projection of otsu_image -----------
					 int VerticalMaxValue=0;
					 for(int i=0;i<=InputW-1;i++)
					 {
						 if(vertical_value[i]>=VerticalMaxValue)
						 {VerticalMaxValue=vertical_value[i];}
					 }
					 //-----------------------------Remove the original image projection of the maximum level-----------------
					 int HorizonMaxValue=0;
					 for(int i=0;i<=InputH-1;i++)
					 {
						 if(horizon_value[i]>=HorizonMaxValue)
						 {HorizonMaxValue=horizon_value[i];}
					 }

					 Mat energy_hist= Mat(RoiW,VerticalMaxValue, img.type(),cv::Scalar( 255, 255, 255 ));
					 for(int i=0;i<=RoiW-1;i++)
					 {
						 line(energy_hist,cv::Point(0,i), cv::Point(vertical_value[i], i), cv::Scalar( 0, 0, 255 ), 1, 1);
					 }
					 delete[] vertical_value;
					 
					 int col_energy_hist = energy_hist.cols;
					 int row_energy_hist = energy_hist.rows;
					 for (int i = energy_hist.cols; i < energy_hist.cols+4; i++) {if (col_energy_hist %4 != 0) col_energy_hist ++;}
					 for (int i = energy_hist.rows; i < energy_hist.rows+4; i++) {if (row_energy_hist %4 != 0) row_energy_hist ++;}
					 Mat energy_hist_s = Mat(row_energy_hist, col_energy_hist, energy_hist.type());
					 resize(energy_hist, energy_hist_s, energy_hist_s.size(), 0, 0, INTER_LINEAR);

					
					 

					


























					 waitKey(0);
					 step++;
					

				 }
			 }
	private: System::Void pB_EAproces_Click(System::Object^  sender, System::EventArgs^  e) {
			 }
};
}

