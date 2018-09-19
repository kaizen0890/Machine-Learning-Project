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

#include <msclr/marshal_cppstd.h>

using namespace System::Runtime::InteropServices;
using namespace cv;
using namespace std;
using namespace msclr::interop;

namespace FourierTransform {

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
	private: System::Windows::Forms::PictureBox^  pB_dft;

	private: System::Windows::Forms::PictureBox^  pB_img;



	private: System::Windows::Forms::PictureBox^  pB_phI;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::PictureBox^  pB_magI;
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::TextBox^  textBox2;
	private: System::Windows::Forms::TextBox^  textBox3;
	private: System::Windows::Forms::TextBox^  textBox4;





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
			this->pB_dft = (gcnew System::Windows::Forms::PictureBox());
			this->pB_img = (gcnew System::Windows::Forms::PictureBox());
			this->pB_phI = (gcnew System::Windows::Forms::PictureBox());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->pB_magI = (gcnew System::Windows::Forms::PictureBox());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->textBox3 = (gcnew System::Windows::Forms::TextBox());
			this->textBox4 = (gcnew System::Windows::Forms::TextBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_dft))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_img))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_phI))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_magI))->BeginInit();
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(25, 127);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(101, 68);
			this->button1->TabIndex = 0;
			this->button1->Text = L"FOURIER TRANSFORM";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// openFileDialog
			// 
			this->openFileDialog->FileName = L"openFileDialog";
			this->openFileDialog->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &Form1::openFileDialog_FileOk);
			// 
			// pB_dft
			// 
			this->pB_dft->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pB_dft->Location = System::Drawing::Point(501, 323);
			this->pB_dft->Name = L"pB_dft";
			this->pB_dft->Size = System::Drawing::Size(333, 231);
			this->pB_dft->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pB_dft->TabIndex = 1;
			this->pB_dft->TabStop = false;
			this->pB_dft->Click += gcnew System::EventHandler(this, &Form1::pB_dft_Click);
			// 
			// pB_img
			// 
			this->pB_img->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pB_img->Location = System::Drawing::Point(163, 323);
			this->pB_img->Name = L"pB_img";
			this->pB_img->Size = System::Drawing::Size(318, 231);
			this->pB_img->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pB_img->TabIndex = 3;
			this->pB_img->TabStop = false;
			// 
			// pB_phI
			// 
			this->pB_phI->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pB_phI->Location = System::Drawing::Point(501, 47);
			this->pB_phI->Name = L"pB_phI";
			this->pB_phI->Size = System::Drawing::Size(333, 213);
			this->pB_phI->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pB_phI->TabIndex = 5;
			this->pB_phI->TabStop = false;
			this->pB_phI->Click += gcnew System::EventHandler(this, &Form1::pB_phI_Click);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(26, 384);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(100, 77);
			this->button2->TabIndex = 6;
			this->button2->Text = L"CHANGE";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &Form1::button2_Click_1);
			// 
			// pB_magI
			// 
			this->pB_magI->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pB_magI->Location = System::Drawing::Point(163, 47);
			this->pB_magI->Name = L"pB_magI";
			this->pB_magI->Size = System::Drawing::Size(318, 213);
			this->pB_magI->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pB_magI->TabIndex = 7;
			this->pB_magI->TabStop = false;
			this->pB_magI->Click += gcnew System::EventHandler(this, &Form1::pB_magI_Click);
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(259, 21);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(128, 20);
			this->textBox1->TabIndex = 8;
			this->textBox1->Text = L"MAGNITUDE OF IMAGE";
			// 
			// textBox2
			// 
			this->textBox2->Location = System::Drawing::Point(615, 21);
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(100, 20);
			this->textBox2->TabIndex = 9;
			this->textBox2->Text = L"PHASE OF IMAGE";
			this->textBox2->TextChanged += gcnew System::EventHandler(this, &Form1::textBox2_TextChanged);
			// 
			// textBox3
			// 
			this->textBox3->Location = System::Drawing::Point(270, 286);
			this->textBox3->Name = L"textBox3";
			this->textBox3->Size = System::Drawing::Size(95, 20);
			this->textBox3->TabIndex = 10;
			this->textBox3->Text = L"ORIGINAL IMAGE";
			// 
			// textBox4
			// 
			this->textBox4->Location = System::Drawing::Point(615, 286);
			this->textBox4->Name = L"textBox4";
			this->textBox4->Size = System::Drawing::Size(100, 20);
			this->textBox4->TabIndex = 11;
			this->textBox4->Text = L"CHANGED IMAGE";
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(883, 566);
			this->Controls->Add(this->textBox4);
			this->Controls->Add(this->textBox3);
			this->Controls->Add(this->textBox2);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->pB_magI);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->pB_phI);
			this->Controls->Add(this->pB_img);
			this->Controls->Add(this->pB_dft);
			this->Controls->Add(this->button1);
			this->Name = L"Form1";
			this->Text = L"Form1";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_dft))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_img))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_phI))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_magI))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

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
						
						Mat img = imread(imgname,CV_LOAD_IMAGE_COLOR);
					
						Mat dst;
						cvtColor(img, dst , CV_BGR2GRAY);
						
						
						// ket thuc mo file anh

// ------------------------------------------FOURIER TRANSFORM-------------------------------------------- 
				 
				 
						Mat padded;                            //expand input image to optimal size
						int m = getOptimalDFTSize(dst.rows);
						int n = getOptimalDFTSize(dst.cols); // on the border add zero values
						copyMakeBorder(dst, padded, 0, m - dst.rows, 0, n - dst.cols, BORDER_CONSTANT, Scalar::all(0));

						Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)}; // tao mien gia tri thuc va ao
						Mat complex;
						merge(planes, 2, complex); 
						Mat complex_out; 
						dft(complex, complex_out , DFT_COMPLEX_OUTPUT | DFT_SCALE);            

					
						// Tao mien gia tri moi sau khi bien doi DFT
						Mat planes_dft[] = {Mat::zeros(padded.size(), CV_32F), Mat::zeros(padded.size(), CV_32F)}; 
						split(complex_out, planes_dft);  
						
						// planes_dft[0] = Re(DFT(I), planes_dft[1] = Im(DFT(I))
						
						
						
						// tinh toan gia tri magnitude va phase
					
						Mat magI = Mat::zeros(padded.size(), CV_32F);
						Mat phI = Mat::zeros(padded.size(), CV_32F);
						magnitude(planes_dft[0], planes_dft[1], magI);
						phase(planes_dft[0] , planes_dft[1] , phI);
				 
					
						 //scale mien gia tri cua Magnitude va phase
						

						//phI += Scalar::all(1);
						//log(phI,phI); 

						//magI += Scalar::all(1);                    // switch to logarithmic scale
						//log(magI, magI);

						//normalize(magI, magI, 1, 0, NORM_INF);
						//normalize(phI, phI, 1, 0, NORM_INF);

//// ------------------------------------- SAP XEP LAI GIA TRI MAGNITUDE VA PHASE DE HIEN THI -----------------------------//
//
				  // crop the spectrum, if it has an odd number of rows or columns

						Mat magI_scale = magI.clone();
						Mat phI_scale = phI.clone();	
					 
						magI_scale = magI_scale(Rect(0, 0, magI_scale.cols & -2, magI_scale.rows & -2));
						phI_scale = phI_scale(Rect(0, 0, phI_scale.cols & -2, phI.rows & -2));

						

				 //rearrange the quadrants of Fourier image  so that the origin is at the image center
					 

						int cx = magI_scale.cols/2;
						int cy = magI_scale.rows/2;

						Mat q0(magI_scale, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
						Mat q1(magI_scale, Rect(cx, 0, cx, cy));  // Top-Right
						Mat q2(magI_scale, Rect(0, cy, cx, cy));  // Bottom-Left
						Mat q3(magI_scale, Rect(cx, cy, cx, cy)); // Bottom-Right

						Mat tmp;                            // swap quadrants (Top-Left with Bottom-Right)
						q0.copyTo(tmp);
						q3.copyTo(q0);
						tmp.copyTo(q3);

						q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
						q2.copyTo(q1);
						tmp.copyTo(q2);			 

						normalize(magI_scale, magI_scale, 0, 255, CV_MINMAX); 
						magI_scale.convertTo(magI_scale, CV_8UC1);

						// normalize gia tri phase 
									 			
										 
 // -------------------------------------THAY DOI GIA TRI CUA MAGNITUDE VA PHASE ---------------------------------//
			
					//	for ( int i = 0 ; i < magI.cols ; i++)
					//			for (int j = 0 ; j < magI.rows; j++)
					//				{				
					//				magI.at<float>(cv::Point(i,j)) =   magI.at<float>(cv::Point(i , j));		
					//				}
					//	for ( int i = 0 ; i < phI.cols ; i++)
					//			for (int j = 0 ; j < phI.rows ; j++)
					//				{							
					//				phI.at<float>(cv::Point(i , j)) = 1.5*phI.at<float>(cv::Point(i , j));
					//				}

     //    




				 //// Tao ma tran loc voi thong so bat ky sau do nhan voi gia tri magnitude va phase 		
					//  Mat kern = (Mat_<char>(3,3) <<  0, -1,  0,
					//							    -1, 4,  -1,
					//								0, -1,  0);
 
					// filter2D(phI, phI, phI.depth(), kern);



//---------------------inverse lai gia tri phan Re va Im tu gia tri Magnitude va phase (sau khi bien doi)----------------------------
		
						for ( int i = 0 ; i < n ; i++)
								for ( int j = 0 ; j < m ; j++)
								{
									planes_dft[0].at<float>(cv::Point(i, j)) = (magI.at<float>(cv::Point(i, j)))*(cos(phI.at<float>(cv::Point(i, j))));		
									planes_dft[1].at<float>(cv::Point(i, j)) = (magI.at<float>(cv::Point(i, j)))*(sin(phI.at<float>(cv::Point(i, j))));
									//invert image by only phase
									// let all magtitude equal 1
									
									/*planes_dft[0].at<float>(cv::Point(i, j)) = (1)*(cos(phI.at<float>(cv::Point(i, j))));
						
									planes_dft[1].at<float>(cv::Point(i, j)) = (1)*(sin(phI.at<float>(cv::Point(i, j))));*/

									/*planes_dft[0].at<float>(cv::Point(i, j)) = (magI.at<float>(cv::Point(i, j)))*(cos(phI.at<float>(cv::Point(i, j))));
						
									planes_dft[1].at<float>(cv::Point(i, j)) = (magI.at<float>(cv::Point(i, j)))*(sin(phI.at<float>(cv::Point(i, j))));*/
								}

					 // ket hop mien gia tri RE va IM vao bien complex_out				 
					 merge(planes_dft , 2 , complex_out);
					 // bien doi DFT nguoc complex_out
					 Mat complex_idft;
					 dft(complex_out, complex_idft, DFT_INVERSE | DFT_REAL_OUTPUT);	
					
					 // Tao anh hien thi gia tri dau ra sau khi bien doi IDFT					  
					 Mat output =  Mat::zeros(img.size(), img.type());
					 complex_idft.convertTo(output, CV_8U);
					
// ----------------------------------------------------- HIEN THI KET QUA TREN PICTURES BOX -------------------------------------//

					
      // ------------------------Hien thi phase ----------------------

			     // Bien doi IDFT thanh phan Phase
					 for ( int i = 0 ; i < n ; i++)
							for ( int j = 0 ; j < m ; j++)
								{						
				 //invert image by only phase
				 // let all magtitude equal 1				
									planes_dft[0].at<float>(cv::Point(i, j)) = (1)*(cos(phI.at<float>(cv::Point(i, j))));		
									planes_dft[1].at<float>(cv::Point(i, j)) = (1)*(sin(phI.at<float>(cv::Point(i, j))));
							     }
					 Mat phI_out;
					 merge(planes_dft , 2 , phI_out);
				 // bien doi DFT nguoc complex_out
					 dft(phI_out, phI_out, DFT_INVERSE | DFT_REAL_OUTPUT);
					 normalize(phI_out,phI_out,0,255, CV_MINMAX); 
					 phI_out.convertTo(phI_out, CV_8U);

				 // ----------------Hien thi Phase-----------						 
					 Mat phI_output;
					 cv::Mat phI_in[] = {phI_out, phI_out, phI_out};
					 cv::merge(phI_in, 3, phI_output); // ghep 3 kenh mau bang nhau magI_1 vao 1 anh magI_out
					 int row_phI = (phI_output.rows/4)*4;
					 int cols_phI = (phI_output.cols/4)*4;
					 Mat phI_output1 = Mat(row_phI, cols_phI, CV_8UC3);
					 resize(phI_output, phI_output1, phI_output1.size(), 0, 0, INTER_LINEAR);
					 
					 pB_phI->Image  = gcnew  
					 System::Drawing::Bitmap(phI_output1.size().width, phI_output1.size().height,phI_output1.step,
					 System::Drawing::Imaging::PixelFormat::Format24bppRgb,(System::IntPtr) phI_output1.data);
					 pB_phI->Refresh();
				     step++;
					 
				 //  ---------------Hien thi Magnitude-------------
	
					 Mat magI_output;				
					 Mat in1[] = {magI_scale, magI_scale, magI_scale};
					 merge(in1, 3, magI_output); // ghep 3 kenh mau bang nhau magI_1 vao 1 anh magI_out
					 int row_magI = (magI_output.rows/4)*4;
					 int cols_magI = (magI_output.cols/4)*4;
					 Mat magI_output1 = Mat(row_magI, cols_magI, CV_8UC3);
					 resize(magI_output, magI_output1, magI_output1.size(), 0, 0, INTER_LINEAR);
					 		 
					 pB_magI->Image  = gcnew  
					 System::Drawing::Bitmap(magI_output1.size().width, magI_output1.size().height,magI_output1.step,
					 System::Drawing::Imaging::PixelFormat::Format24bppRgb,(System::IntPtr) magI_output1.data);
					 pB_magI->Refresh();
				     step++;

					 // ------------------Hien thi anh dau ra---------------------

					// picturesBox luon hien thi anh 3 kenh mau BGR nen khi muon hien thi, ta phai tao anh moi magI_out bao gom 3 kenh mau magI_1 bang nhau
					 Mat result_out;
				
					 Mat in2[] = {output, output, output};
					 merge(in2, 3, result_out); // ghep 3 kenh mau bang nhau magI_1 vao 1 anh magI_out

					 int row_output = (result_out.rows/4)*4;
					 int cols_output = (result_out.cols/4)*4;
					 Mat result_out1 = Mat(row_output, cols_output, CV_8UC3);
					 resize(result_out, result_out1, result_out1.size(), 0, 0, INTER_LINEAR);

					 pB_dft->Image  = gcnew  
					 System::Drawing::Bitmap(result_out1.size().width, result_out1.size().height,result_out1.step,
					 System::Drawing::Imaging::PixelFormat::Format24bppRgb,(System::IntPtr) result_out1.data);
					 pB_dft->Refresh();
				     step++;

					 //---------Hien thi anh goc ------------------
					 int row_s = (img.rows/4)*4;
					 int cols_s = (img.cols/4)*4;

					 Mat src_s = Mat(row_s, cols_s, CV_8UC3);
					 resize(img, src_s, src_s.size(), 0, 0, INTER_LINEAR);
					 
					 pB_img->Image  = gcnew  
					 System::Drawing::Bitmap(src_s.size().width,src_s.size().height,src_s.step,
					 System::Drawing::Imaging::PixelFormat::Format24bppRgb,(System::IntPtr) src_s.data);
					 pB_img->Refresh();
				     step++;

					}
					
			 }
	private: System::Void openFileDialog_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e) {
			 }
	private: System::Void btn_load_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				  
			 }
private: System::Void pictureBox2_Click(System::Object^  sender, System::EventArgs^  e)
		 {

			 

		 }
private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) 
		 {

			

		 }
private: System::Void button2_Click_1(System::Object^  sender, System::EventArgs^  e) 

		 {


////
		 }
private: System::Void pB_dft_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void pB_magI_Click(System::Object^  sender, System::EventArgs^  e) 
		 {



		 }
private: System::Void pB_phI_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void textBox2_TextChanged(System::Object^  sender, System::EventArgs^  e) {
		 }
};
}

