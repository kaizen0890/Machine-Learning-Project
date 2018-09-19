#pragma once


#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <math.h>
#include <opencv2\core\core.hpp>
#include <opencv2\core\core_c.h>
#include <opencv2\objdetect\objdetect.hpp>
#include <string.h>
#include <conio.h>
#include <stdio.h>

#include <opencv2\wavelib\wavelet2d.h>


#include <msclr/marshal_cppstd.h>

using namespace System::Runtime::InteropServices;
using namespace cv;
using namespace std;
using namespace msclr::interop;



static double mean_pos(Mat img)
{
	double mean = 0;
	for (int i=0; i < img.rows; i++) 
	{
		for (int j =0; j < img.cols; j++)
		{
			if (img.at<double>(cv::Point(j, i)) >= 0) {mean+=img.at<double>(cv::Point(j, i));}

		}

	}
	mean/=(img.rows*img.cols);
	return mean;

}
static void* max_array(vector<vector<double> > &arr, double &max)
{
	max = 0;
	for (unsigned int i =0; i < arr.size(); i++) 
	{
		for (unsigned int j =0; j < arr[0].size(); j++) 
		{
			if (max <= arr[i][j])
			{
				max = arr[i][j];
			}
		}
	}
	return 0;
}

namespace WaveletTransform {

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
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog;
	private: System::Windows::Forms::PictureBox^  pB_wav;

	protected: 

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
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->openFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());
			this->pB_wav = (gcnew System::Windows::Forms::PictureBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_wav))->BeginInit();
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(47, 47);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(87, 56);
			this->button1->TabIndex = 0;
			this->button1->Text = L"Open";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// openFileDialog
			// 
			this->openFileDialog->FileName = L"openFileDialog";
			this->openFileDialog->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &Form1::openFileDialog1_FileOk);
			// 
			// pB_wav
			// 
			this->pB_wav->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pB_wav->Location = System::Drawing::Point(230, 48);
			this->pB_wav->Name = L"pB_wav";
			this->pB_wav->Size = System::Drawing::Size(329, 317);
			this->pB_wav->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pB_wav->TabIndex = 1;
			this->pB_wav->TabStop = false;
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(790, 548);
			this->Controls->Add(this->pB_wav);
			this->Controls->Add(this->button1);
			this->Name = L"Form1";
			this->Text = L"Form1";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_wav))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e)
			 
			 {
				 //  openFileDialog
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
					System::String^ OpenFileName;
					int step = 0;
					while (step<number_of_files)
					{
						OpenFileName = filename_src[step];//duong dan day du, kieu du lieu System::String^
				
						OpenFileName = OpenFileName->Replace("\\","\\\\");// opencv pathfile is C:\\folder\\filename
						

						char* imgname;
						imgname = (char*)Marshal::StringToHGlobalAnsi(OpenFileName).ToPointer();
						
						Mat img = cv::imread(imgname,1);
						
						

						int rows = img.rows;
						int cols = img.cols;

						int count_1 = 0;
						int count_2 = 0;
						int count_3 = 0;



						Mat img_src;
						img.convertTo(img_src, CV_32F);

						vector<vector<double> > vec_R(rows, vector<double>(cols));
						vector<vector<double> > vec_G(rows, vector<double>(cols));
						vector<vector<double> > vec_B(rows, vector<double>(cols));
						vector<vector<double> > vec_Ga(rows, vector<double>(cols));

						Mat B = Mat(img_src.size(), CV_32F);
						Mat G = Mat(img_src.size(), CV_32F);
						Mat R = Mat(img_src.size(), CV_32F);

						for (int i = 0;i<img_src.cols;i++)
						{
							for (int j =0; j<img_src.rows;j++)

							{

								B.at<float>(cv::Point(i, j)) = img_src.at<Vec3f>(cv::Point(i, j))[0];
								G.at<float>(cv::Point(i, j)) = img_src.at<Vec3f>(cv::Point(i, j))[1];
								R.at<float>(cv::Point(i, j)) = img_src.at<Vec3f>(cv::Point(i, j))[2];


								vec_B[j][i] = (double) B.at<float>(cv::Point(i, j));
								vec_G[j][i] = (double) G.at<float>(cv::Point(i, j));
								vec_R[j][i] = (double) R.at<float>(cv::Point(i, j));

							}
						}


						float pLP_img = (count_1/(float)(img.cols*img.rows))*100.0; // percent low point
						float pNP_img = (count_2/(float)(img.cols*img.rows))*100.0; // percent normal point
						float pHP_img = (count_3/(float)(img.cols*img.rows))*100.0; // percent hight point

						double mi; 
						if (pLP_img > 50) {mi = 208;}
						else if (pHP_img > 50) {mi = 98;} else {mi = 160;};


						Mat Ga = Mat(rows, cols, CV_64F);
						double stdGa = sqrtf(32);
						randn(Ga, mi, stdGa);

						for (int i=0; i < rows; i++) 
						{
							for (int j =0; j < cols; j++)
							{
								vec_Ga[i][j] = (double) Ga.at<double>(cv::Point(j, i));						
							}

						}

						vector<int> length_B, length_G, length_R;
						vector<double> output_B, flag, output_ASVD_B;
						vector<double> output_G, output_ASVD_G;
						vector<double> output_R, output_ASVD_R;
						vector<double> output_Ga;

						int J = 2;
						string nm = "db1";
						

						dwt_2d(vec_B,J,nm,output_B,flag,length_B);
						dwt_2d(vec_G,J,nm,output_G,flag,length_G);
						dwt_2d(vec_R,J,nm,output_R,flag,length_R);
						dwt_2d(vec_Ga,J,nm,output_Ga,flag,length_R);

						// Compute wavelet subband image (for display)
						vector<int> length2_B; // luu tru kich thuoc cua anh
						dwt_output_dim_sym(length_B,length2_B,J);

						int siz = length2_B.size();
						int rows_n=length2_B[siz-2];
						int cols_n = length2_B[siz-1];

						vector<vector< double> > dwtdisp_B(rows_n, vector<double>(cols_n));
						vector<vector< double> > dwtdisp_G(rows_n, vector<double>(cols_n));
						vector<vector< double> > dwtdisp_R(rows_n, vector<double>(cols_n));

						dispDWT(output_B, dwtdisp_B, length_B ,length2_B, J);
						dispDWT(output_G, dwtdisp_G, length_B ,length2_B, J);
						dispDWT(output_R, dwtdisp_R, length_B ,length2_B, J);



						vector<vector<double> >  dwt_output_B= dwtdisp_B;
						vector<vector<double> >  dwt_output_G= dwtdisp_G;
						vector<vector<double> >  dwt_output_R= dwtdisp_R;
						double max_B, max_G, max_R;
						max_array(dwt_output_B,max_B);// max value is needed to take care of overflow
						max_array(dwt_output_G,max_G);
						max_array(dwt_output_R,max_R);

						cv::Mat img_dwt = Mat::zeros(rows_n,cols_n, img_src.type());

						// dwt_hold is created to hold the dwt output as further operations need to be
						// carried out on dwt_output in order to display scaled images.

						vector<vector<double> > dwt_hold_B(rows_n, vector<double>( cols_n));
						vector<vector<double> > dwt_hold_G(rows_n, vector<double>( cols_n));
						vector<vector<double> > dwt_hold_R(rows_n, vector<double>( cols_n));
						dwt_hold_B = dwt_output_B;
						dwt_hold_G = dwt_output_G;
						dwt_hold_R = dwt_output_R;

						// Setting coefficients of created image to the scaled DWT output values
						for (int i = 0; i < img_dwt.rows; i++ ) 
						{
							for (int j = 0; j < img_dwt.cols; j++ )
							{
								if ( dwt_output_B[i][j] <= 0.0)
								{
									dwt_output_B[i][j] = 0.0;
								}
								if ( dwt_output_G[i][j] <= 0.0)
								{
									dwt_output_G[i][j] = 0.0;
								}
								if ( dwt_output_R[i][j] <= 0.0)
								{
									dwt_output_R[i][j] = 0.0;
								}

								if ( i <= (length2_B[0]) && j <= (length2_B[1]) ) 
								{

									img_dwt.at<Vec3f>(cv::Point(j, i))[0] = (float) ( (dwt_output_B[i][j] / max_B) * 255.0);
									img_dwt.at<Vec3f>(cv::Point(j, i))[1] = (float) ( (dwt_output_G[i][j] / max_G) * 255.0);
									img_dwt.at<Vec3f>(cv::Point(j, i))[2] = (float) ( (dwt_output_R[i][j] / max_R) * 255.0);
								} 
								else 
								{
									img_dwt.at<Vec3f>(cv::Point(j, i))[0] = (float) (dwt_output_B[i][j]);
									img_dwt.at<Vec3f>(cv::Point(j, i))[1] = (float) (dwt_output_G[i][j]);
									img_dwt.at<Vec3f>(cv::Point(j, i))[2] = (float) (dwt_output_R[i][j]);
								}
							}
						}
						img_dwt.convertTo(img_dwt, CV_8UC3);
						Mat output_LL;
						int rows_1 = length_B[0];
						int cols_1 = length_B[1];
						Mat LL = Mat (rows_1, cols_1, CV_64F);
						for (int k = 0; k <=  rows_1*cols_1; k++)
						{
							int i = k/cols_1 + 1;
							int j = k%rows_1 + 1;
							LL.at<double>(cv::Point(i,j))= output_B[k];

						}
						for (int i = 0; i<= LL.rows; i++)
							for (int j = 0; j<= LL.cols; j++)
							
						{
							if (LL.at<double>(cv::Point(i,j)) < 0)
							{
								LL.at<double>(cv::Point(i,j)) +=  abs( LL.at<double>(cv::Point(i,j))) ;
							}
						}
						
						LL.convertTo(LL, CV_8UC3);


						
						imshow("", LL);
						


					
							
							
							


						
				

						// Finding IDWT

					/*	vector<vector<double> > idwt_output(rows, vector<double>(cols));
						idwt_2d_sym(output,flag, nm, idwt_output,length);
				
						cv::Mat idwt_img = Mat::zeros(idwt_output.size(),idwt_output[0].size(), img.type());
						for (int i = 0; i < idwt_img.rows; i++ )
							for (int j = 0; j < idwt_img.cols; j++ )
							{
								idwt_img.at<unsigned char>(cv::Point(j, i)) = (char) (idwt_output[i][j]);
							}*/
				
							
						
					
						int row_s = (img.rows/4)*4;
						int col_s = (img.cols/4)*4;
						Mat img_s = Mat(row_s, col_s, img.type());

						Mat img_dwt_disp = Mat(row_s, col_s, img.type());
						resize(img_dwt, img_dwt_disp, img_dwt_disp.size(), 0, 0, INTER_LINEAR);
							

						pB_wav->Image  = gcnew    
						System::Drawing::Bitmap(img_dwt_disp.size().width,img_dwt_disp.size().height,img_dwt_disp.step,
						System::Drawing::Imaging::PixelFormat::Format24bppRgb,(System::IntPtr) img_dwt_disp.data);
						pB_wav->Refresh();


						/*int show_rows = img_temp.rows/2;
						int show_cols = img_temp.cols/2;
						Mat show = Mat::zeros(show.rows,show.cols, img.type());;
						
												
								
										
								

							

						show.convertTo(show, CV_8UC1);
						imshow("", show);

*/



						waitKey();
						return;
					}


			 }
	private: System::Void openFileDialog1_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e) {
			 }
};
}

