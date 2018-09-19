#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <fstream> // lam viec voi file text
#include <sstream>
#include <stdlib.h>
#include <Windows.h>
#include <msclr/marshal_cppstd.h>


#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>




using namespace std;
using namespace cv;


using namespace System::Runtime::InteropServices;

using namespace msclr::interop;
using namespace System::Data;
using namespace System::Drawing;



namespace SVD {

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
	private: System::Windows::Forms::Button^  bt_1;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog;
	private: System::Windows::Forms::PictureBox^  pB_output;
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
			this->bt_1 = (gcnew System::Windows::Forms::Button());
			this->openFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());
			this->pB_output = (gcnew System::Windows::Forms::PictureBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_output))->BeginInit();
			this->SuspendLayout();
			// 
			// bt_1
			// 
			this->bt_1->Location = System::Drawing::Point(91, 77);
			this->bt_1->Name = L"bt_1";
			this->bt_1->Size = System::Drawing::Size(105, 66);
			this->bt_1->TabIndex = 0;
			this->bt_1->Text = L"Open";
			this->bt_1->UseVisualStyleBackColor = true;
			this->bt_1->Click += gcnew System::EventHandler(this, &Form1::bt_1_Click);
			// 
			// openFileDialog
			// 
			this->openFileDialog->FileName = L"openFileDialog";
			// 
			// pB_output
			// 
			this->pB_output->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pB_output->Location = System::Drawing::Point(411, 51);
			this->pB_output->Name = L"pB_output";
			this->pB_output->Size = System::Drawing::Size(349, 304);
			this->pB_output->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pB_output->TabIndex = 1;
			this->pB_output->TabStop = false;
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1062, 441);
			this->Controls->Add(this->pB_output);
			this->Controls->Add(this->bt_1);
			this->Name = L"Form1";
			this->Text = L"Form1";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_output))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void bt_1_Click(System::Object^  sender, System::EventArgs^  e) {




				 // ----------------OPEN FILE IMAGE---------------
				 openFileDialog->Filter = "Image Files|*.bmp; *.jpg; *.png|All Files (*.*)|*.*||";  openFileDialog->Title = "Select a image file"; 
				 openFileDialog->InitialDirectory = "";
				 openFileDialog-> Multiselect = "True"; 

				 int number_of_files;
				 array<System::String^>^ filename_src; // array of filenames
				 if ( openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK )  
				 {
					 filename_src = openFileDialog->FileNames;
					 number_of_files = openFileDialog->FileNames->Length;

				 }
				 else return;
				 System::String^ OpenFileName;
				 System::String^ step_str;
				 int step = 0;
				 while (step <number_of_files)

				 {	
					 OpenFileName = filename_src[step];	
					 OpenFileName = OpenFileName->Replace("\\","\\\\");
					 char* imgname = (char*)Marshal::StringToHGlobalAnsi(OpenFileName).ToPointer();  // convert to kieu char lay ten filecho opencv	
					 Mat img = cv::imread(imgname);

					 


					 // Ket thuc mo file



					 /////////// SVD transform /////////


					 int m = img.rows;
					 int n = img.cols;

					 ///////////// Create (float) Mat to store color channels ////////////

					 Mat B = Mat_<float>(m,n); 
					 Mat G = Mat_<float>(m,n);
					 Mat R = Mat_<float>(m,n);
					 Mat img_src;

					 /////////////// Have to convert Image from 8Uc1 to CV_32F //////////////
					 img.convertTo(img_src, CV_32F);
					 
					 ////////////// Spit image to 3 color channels //////////////
					 for (int i = 0;i<img.cols;i++)
					 {
						 for (int j =0; j<img.rows;j++)
						 {

							 B.at<float>(cv::Point(i, j)) = img_src.at<Vec3f>(cv::Point(i, j))[0];
							 G.at<float>(cv::Point(i, j)) = img_src.at<Vec3f>(cv::Point(i, j))[1];
							 R.at<float>(cv::Point(i, j)) = img_src.at<Vec3f>(cv::Point(i, j))[2];
						 }
					 }

					 
					 //////// SVD transform /////////
					 cv::SVD svd(B);  //// create structure SVD and implement SVD transform on B image


					 Mat w, u, vt;
					 cv::SVD::compute(R,w,u,vt,cv::SVD::FULL_UV);


					 //////// SVD Inverse transform /////////
					 Mat B_svd = (svd.u)*Mat::diag(svd.w)*svd.vt; 
					 
					 //////// To display image, have to convert image from CV_32F to CV_8UC1 
					 svd.w.convertTo(svd.u, CV_8UC1);
					 B_svd.convertTo(B_svd, CV_8UC1);
					 imshow("",svd.w);




					 
			

					 

					 


					 //////// SVD inverse ///////////

					 

					 






					




























					 waitKey(0);
					 return;

				 }

				 step++;

















			 }
	};
}

