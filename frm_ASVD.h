#pragma once

#include <fstream> // lam viec voi file text
#include <Windows.h>

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/contrib/contrib.hpp"
#include <iostream>
#include <msclr/marshal_cppstd.h>

#include <sstream>
#include <conio.h>



using namespace System::Runtime::InteropServices;
using namespace cv;
using namespace std;
using namespace msclr::interop;
using namespace System::Data;
using namespace System::Drawing;

// Tinh thoi gian
#define TIMER_INIT LARGE_INTEGER frequency;LARGE_INTEGER t1,t2;double elapsedTime;QueryPerformanceFrequency(&frequency);

// Use to start the performance timer /
#define TIMER_START QueryPerformanceCounter(&t1);

//** Use to stop the performance timer and output the result to the standard stream. Less verbose than \c TIMER_STOP_VERBOSE /
#define TIMER_STOP QueryPerformanceCounter(&t2);elapsedTime=(float)(t2.QuadPart-t1.QuadPart)/frequency.QuadPart;//std::wcout<<elapsedTime<<L" sec"<<endl;
//Ket thuc tinh thoi gian
////////////////////////
////////////////////////
static Mat drawhistogram(Mat img)
{
	// draw histogram
	/// Separate the image in 3 places ( B, G and R )
	vector<Mat> bgr_planes;
	split( img, bgr_planes );

	/// Establish the number of bins
	int histSize = 256;

	/// Set the ranges ( for B,G,R) )
	float range[] = { 0, 255 } ;
	const float* histRange = { range };

	bool uniform = true; bool accumulate = false;

	Mat b_hist, g_hist, r_hist;

	/// Compute the histograms:
	calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
	calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
	calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );

	// Draw the histograms for B, G and R
	int hist_w = 1024; int hist_h = 400;
	int bin_w = cvRound( (double) hist_w/histSize );

	Mat histImage( hist_h, hist_w, CV_8UC3, Scalar(255, 255, 255) );//white, (0,0,0) is black
				
	/// Normalize the result to [ 0, histImage.rows ]
	normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
	normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
	normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

	// Draw for each channel
	for( int i = 1; i < histSize; i++ )
	{
		line( histImage, cv::Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
						 cv::Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
                         Scalar( 255, 0, 0), 2, 8, 0  );//blue
		line( histImage, cv::Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
						 cv::Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
                         Scalar( 0, 255, 0), 2, 8, 0  );//green
		line( histImage, cv::Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
                         cv::Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
                         Scalar( 0, 0, 255), 2, 8, 0  );//red
	}
	return histImage;		
}
///////////////


namespace ASVD {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for frm_ASVD
	/// </summary>
	public ref class frm_ASVD : public System::Windows::Forms::Form
	{
	public:
		frm_ASVD(void)
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
		~frm_ASVD()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  lab_result;
	protected: 
	private: System::Windows::Forms::Label^  lab_ori;
	private: System::Windows::Forms::PictureBox^  pB_hist_dst;
	private: System::Windows::Forms::PictureBox^  pB_hist_src;

	private: System::Windows::Forms::PictureBox^  pB_ASVD;
	private: System::Windows::Forms::PictureBox^  pB_ori;
	private: System::Windows::Forms::Button^  btn_ASVD;
	private: System::Windows::Forms::Button^  btn_RD;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog;
	private: System::Windows::Forms::Button^  btn_PCS;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::NumericUpDown^  nUD_sigma;
	private: System::Windows::Forms::NumericUpDown^  nUD_mGa;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::TextBox^  txt_xi_B;

	private: System::Windows::Forms::Label^  label11;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::TextBox^  txt_maxwGa;


	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::TextBox^  txt_maxwB;
	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::Label^  label12;
	private: System::Windows::Forms::Label^  label13;
	private: System::Windows::Forms::Label^  label14;
	private: System::Windows::Forms::TextBox^  txt_xi_G;
	private: System::Windows::Forms::TextBox^  txt_maxwGa1;
	private: System::Windows::Forms::TextBox^  txt_maxwG;
	private: System::Windows::Forms::Label^  label15;
	private: System::Windows::Forms::Label^  label16;
	private: System::Windows::Forms::Label^  label17;
	private: System::Windows::Forms::Label^  label18;
	private: System::Windows::Forms::Label^  label19;
	private: System::Windows::Forms::TextBox^  txt_xi_R;
	private: System::Windows::Forms::TextBox^  txt_maxwGa2;
	private: System::Windows::Forms::TextBox^  txt_maxwR;
	private: System::Windows::Forms::Label^  label20;
	private: System::Windows::Forms::Label^  label21;
	private: System::Windows::Forms::Label^  label22;
	private: System::Windows::Forms::Label^  label23;
	private: System::Windows::Forms::Label^  label24;
	private: System::Windows::Forms::TextBox^  txt_xiB;
	private: System::Windows::Forms::TextBox^  txt_maxwGa4;
	private: System::Windows::Forms::TextBox^  txt_maxwB1;



	private: System::Windows::Forms::Label^  label25;
	private: System::Windows::Forms::Label^  label26;
	private: System::Windows::Forms::TextBox^  txt_max_mean;

	private: System::Windows::Forms::Label^  label27;
	private: System::Windows::Forms::Label^  label28;
	private: System::Windows::Forms::TextBox^  txt_meanB_float;

	private: System::Windows::Forms::Label^  label29;
	private: System::Windows::Forms::Label^  label30;
	private: System::Windows::Forms::Label^  label31;
	private: System::Windows::Forms::Label^  label32;
	private: System::Windows::Forms::Label^  label33;
	private: System::Windows::Forms::Label^  label34;
	private: System::Windows::Forms::Label^  label35;
	private: System::Windows::Forms::Label^  label36;
	private: System::Windows::Forms::Label^  label37;
	private: System::Windows::Forms::Label^  label38;
	private: System::Windows::Forms::Label^  label39;
	private: System::Windows::Forms::Label^  label40;
	private: System::Windows::Forms::Label^  label41;
	private: System::Windows::Forms::Label^  label42;
	private: System::Windows::Forms::Label^  label43;
	private: System::Windows::Forms::Label^  label44;
	private: System::Windows::Forms::Label^  label45;
	private: System::Windows::Forms::TextBox^  txt_xiG;
private: System::Windows::Forms::TextBox^  txt_maxwGa5;


private: System::Windows::Forms::TextBox^  txt_max_mean1;
private: System::Windows::Forms::TextBox^  txt_meanG_float;
private: System::Windows::Forms::TextBox^  txt_maxwG1;



	private: System::Windows::Forms::Label^  label46;
	private: System::Windows::Forms::Label^  label47;
	private: System::Windows::Forms::Label^  label48;
	private: System::Windows::Forms::Label^  label49;
	private: System::Windows::Forms::Label^  label50;
	private: System::Windows::Forms::Label^  label51;
	private: System::Windows::Forms::Label^  label52;
	private: System::Windows::Forms::Label^  label53;
	private: System::Windows::Forms::Label^  label54;
	private: System::Windows::Forms::Label^  label55;
	private: System::Windows::Forms::Label^  label56;
	private: System::Windows::Forms::Label^  label57;
	private: System::Windows::Forms::Label^  label58;
private: System::Windows::Forms::TextBox^  txt_xiR;
private: System::Windows::Forms::TextBox^  txt_maxwGa6;


private: System::Windows::Forms::TextBox^  txt_max_mean2;
private: System::Windows::Forms::TextBox^  txt_meanR_float;
private: System::Windows::Forms::TextBox^  txt_maxwR1;
private: System::Windows::Forms::GroupBox^  ASVD;
private: System::Windows::Forms::GroupBox^  groupBox1;
private: System::Windows::Forms::Panel^  panel1;





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
			this->lab_result = (gcnew System::Windows::Forms::Label());
			this->lab_ori = (gcnew System::Windows::Forms::Label());
			this->pB_hist_dst = (gcnew System::Windows::Forms::PictureBox());
			this->pB_hist_src = (gcnew System::Windows::Forms::PictureBox());
			this->pB_ASVD = (gcnew System::Windows::Forms::PictureBox());
			this->pB_ori = (gcnew System::Windows::Forms::PictureBox());
			this->btn_ASVD = (gcnew System::Windows::Forms::Button());
			this->btn_RD = (gcnew System::Windows::Forms::Button());
			this->openFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());
			this->btn_PCS = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->nUD_sigma = (gcnew System::Windows::Forms::NumericUpDown());
			this->nUD_mGa = (gcnew System::Windows::Forms::NumericUpDown());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->txt_xi_B = (gcnew System::Windows::Forms::TextBox());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->txt_maxwGa = (gcnew System::Windows::Forms::TextBox());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->txt_maxwB = (gcnew System::Windows::Forms::TextBox());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->txt_xi_G = (gcnew System::Windows::Forms::TextBox());
			this->txt_maxwGa1 = (gcnew System::Windows::Forms::TextBox());
			this->txt_maxwG = (gcnew System::Windows::Forms::TextBox());
			this->label15 = (gcnew System::Windows::Forms::Label());
			this->label16 = (gcnew System::Windows::Forms::Label());
			this->label17 = (gcnew System::Windows::Forms::Label());
			this->label18 = (gcnew System::Windows::Forms::Label());
			this->label19 = (gcnew System::Windows::Forms::Label());
			this->txt_xi_R = (gcnew System::Windows::Forms::TextBox());
			this->txt_maxwGa2 = (gcnew System::Windows::Forms::TextBox());
			this->txt_maxwR = (gcnew System::Windows::Forms::TextBox());
			this->label20 = (gcnew System::Windows::Forms::Label());
			this->label21 = (gcnew System::Windows::Forms::Label());
			this->label22 = (gcnew System::Windows::Forms::Label());
			this->label23 = (gcnew System::Windows::Forms::Label());
			this->label24 = (gcnew System::Windows::Forms::Label());
			this->txt_xiB = (gcnew System::Windows::Forms::TextBox());
			this->txt_maxwGa4 = (gcnew System::Windows::Forms::TextBox());
			this->txt_maxwB1 = (gcnew System::Windows::Forms::TextBox());
			this->label25 = (gcnew System::Windows::Forms::Label());
			this->label26 = (gcnew System::Windows::Forms::Label());
			this->txt_max_mean = (gcnew System::Windows::Forms::TextBox());
			this->label27 = (gcnew System::Windows::Forms::Label());
			this->label28 = (gcnew System::Windows::Forms::Label());
			this->txt_meanB_float = (gcnew System::Windows::Forms::TextBox());
			this->label29 = (gcnew System::Windows::Forms::Label());
			this->label30 = (gcnew System::Windows::Forms::Label());
			this->label31 = (gcnew System::Windows::Forms::Label());
			this->label32 = (gcnew System::Windows::Forms::Label());
			this->label33 = (gcnew System::Windows::Forms::Label());
			this->label34 = (gcnew System::Windows::Forms::Label());
			this->label35 = (gcnew System::Windows::Forms::Label());
			this->label36 = (gcnew System::Windows::Forms::Label());
			this->label37 = (gcnew System::Windows::Forms::Label());
			this->label38 = (gcnew System::Windows::Forms::Label());
			this->label39 = (gcnew System::Windows::Forms::Label());
			this->label40 = (gcnew System::Windows::Forms::Label());
			this->label41 = (gcnew System::Windows::Forms::Label());
			this->label42 = (gcnew System::Windows::Forms::Label());
			this->label43 = (gcnew System::Windows::Forms::Label());
			this->label44 = (gcnew System::Windows::Forms::Label());
			this->label45 = (gcnew System::Windows::Forms::Label());
			this->txt_xiG = (gcnew System::Windows::Forms::TextBox());
			this->txt_maxwGa5 = (gcnew System::Windows::Forms::TextBox());
			this->txt_max_mean1 = (gcnew System::Windows::Forms::TextBox());
			this->txt_meanG_float = (gcnew System::Windows::Forms::TextBox());
			this->txt_maxwG1 = (gcnew System::Windows::Forms::TextBox());
			this->label46 = (gcnew System::Windows::Forms::Label());
			this->label47 = (gcnew System::Windows::Forms::Label());
			this->label48 = (gcnew System::Windows::Forms::Label());
			this->label49 = (gcnew System::Windows::Forms::Label());
			this->label50 = (gcnew System::Windows::Forms::Label());
			this->label51 = (gcnew System::Windows::Forms::Label());
			this->label52 = (gcnew System::Windows::Forms::Label());
			this->label53 = (gcnew System::Windows::Forms::Label());
			this->label54 = (gcnew System::Windows::Forms::Label());
			this->label55 = (gcnew System::Windows::Forms::Label());
			this->label56 = (gcnew System::Windows::Forms::Label());
			this->label57 = (gcnew System::Windows::Forms::Label());
			this->label58 = (gcnew System::Windows::Forms::Label());
			this->txt_xiR = (gcnew System::Windows::Forms::TextBox());
			this->txt_maxwGa6 = (gcnew System::Windows::Forms::TextBox());
			this->txt_max_mean2 = (gcnew System::Windows::Forms::TextBox());
			this->txt_meanR_float = (gcnew System::Windows::Forms::TextBox());
			this->txt_maxwR1 = (gcnew System::Windows::Forms::TextBox());
			this->ASVD = (gcnew System::Windows::Forms::GroupBox());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_hist_dst))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_hist_src))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_ASVD))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_ori))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUD_sigma))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUD_mGa))->BeginInit();
			this->ASVD->SuspendLayout();
			this->groupBox1->SuspendLayout();
			this->panel1->SuspendLayout();
			this->SuspendLayout();
			// 
			// lab_result
			// 
			this->lab_result->AutoSize = true;
			this->lab_result->Location = System::Drawing::Point(614, 244);
			this->lab_result->Name = L"lab_result";
			this->lab_result->Size = System::Drawing::Size(69, 13);
			this->lab_result->TabIndex = 51;
			this->lab_result->Text = L"Result Image";
			// 
			// lab_ori
			// 
			this->lab_ori->AutoSize = true;
			this->lab_ori->Location = System::Drawing::Point(607, 33);
			this->lab_ori->Name = L"lab_ori";
			this->lab_ori->Size = System::Drawing::Size(74, 13);
			this->lab_ori->TabIndex = 52;
			this->lab_ori->Text = L"Original Image";
			// 
			// pB_hist_dst
			// 
			this->pB_hist_dst->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->pB_hist_dst->Location = System::Drawing::Point(742, 265);
			this->pB_hist_dst->Name = L"pB_hist_dst";
			this->pB_hist_dst->Size = System::Drawing::Size(400, 180);
			this->pB_hist_dst->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pB_hist_dst->TabIndex = 50;
			this->pB_hist_dst->TabStop = false;
			// 
			// pB_hist_src
			// 
			this->pB_hist_src->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->pB_hist_src->Location = System::Drawing::Point(742, 61);
			this->pB_hist_src->Name = L"pB_hist_src";
			this->pB_hist_src->Size = System::Drawing::Size(400, 180);
			this->pB_hist_src->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pB_hist_src->TabIndex = 49;
			this->pB_hist_src->TabStop = false;
			// 
			// pB_ASVD
			// 
			this->pB_ASVD->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pB_ASVD->Location = System::Drawing::Point(556, 265);
			this->pB_ASVD->Name = L"pB_ASVD";
			this->pB_ASVD->Size = System::Drawing::Size(180, 180);
			this->pB_ASVD->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pB_ASVD->TabIndex = 46;
			this->pB_ASVD->TabStop = false;
			// 
			// pB_ori
			// 
			this->pB_ori->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pB_ori->Location = System::Drawing::Point(556, 61);
			this->pB_ori->Name = L"pB_ori";
			this->pB_ori->Size = System::Drawing::Size(180, 180);
			this->pB_ori->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pB_ori->TabIndex = 47;
			this->pB_ori->TabStop = false;
			// 
			// btn_ASVD
			// 
			this->btn_ASVD->Location = System::Drawing::Point(181, 15);
			this->btn_ASVD->Name = L"btn_ASVD";
			this->btn_ASVD->Size = System::Drawing::Size(110, 38);
			this->btn_ASVD->TabIndex = 45;
			this->btn_ASVD->Text = L"ASVD";
			this->btn_ASVD->UseVisualStyleBackColor = true;
			this->btn_ASVD->Click += gcnew System::EventHandler(this, &frm_ASVD::btn_ASVD_Click);
			// 
			// btn_RD
			// 
			this->btn_RD->Location = System::Drawing::Point(65, 15);
			this->btn_RD->Name = L"btn_RD";
			this->btn_RD->Size = System::Drawing::Size(110, 38);
			this->btn_RD->TabIndex = 45;
			this->btn_RD->Text = L"R&&D";
			this->btn_RD->UseVisualStyleBackColor = true;
			this->btn_RD->Click += gcnew System::EventHandler(this, &frm_ASVD::btn_RD_Click);
			// 
			// openFileDialog
			// 
			this->openFileDialog->FileName = L"openFileDialog";
			// 
			// btn_PCS
			// 
			this->btn_PCS->Location = System::Drawing::Point(308, 14);
			this->btn_PCS->Name = L"btn_PCS";
			this->btn_PCS->Size = System::Drawing::Size(110, 38);
			this->btn_PCS->TabIndex = 53;
			this->btn_PCS->Text = L"PCS";
			this->btn_PCS->UseVisualStyleBackColor = true;
			this->btn_PCS->Click += gcnew System::EventHandler(this, &frm_ASVD::btn_PCS_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(885, 33);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(124, 13);
			this->label1->TabIndex = 52;
			this->label1->Text = L"Original Image Histogram";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(885, 244);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(119, 13);
			this->label2->TabIndex = 52;
			this->label2->Text = L"Result Image Histogram";
			// 
			// nUD_sigma
			// 
			this->nUD_sigma->Location = System::Drawing::Point(339, 70);
			this->nUD_sigma->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000, 0, 0, 0});
			this->nUD_sigma->Name = L"nUD_sigma";
			this->nUD_sigma->Size = System::Drawing::Size(79, 20);
			this->nUD_sigma->TabIndex = 66;
			// 
			// nUD_mGa
			// 
			this->nUD_mGa->Location = System::Drawing::Point(149, 70);
			this->nUD_mGa->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000, 0, 0, 0});
			this->nUD_mGa->Name = L"nUD_mGa";
			this->nUD_mGa->Size = System::Drawing::Size(64, 20);
			this->nUD_mGa->TabIndex = 67;
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(236, 72);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(97, 13);
			this->label7->TabIndex = 64;
			this->label7->Text = L"Standard Gaussian";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(62, 72);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(81, 13);
			this->label3->TabIndex = 65;
			this->label3->Text = L"Mean Gaussian";
			// 
			// txt_xi_B
			// 
			this->txt_xi_B->Location = System::Drawing::Point(122, 34);
			this->txt_xi_B->Name = L"txt_xi_B";
			this->txt_xi_B->Size = System::Drawing::Size(46, 20);
			this->txt_xi_B->TabIndex = 69;
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Location = System::Drawing::Point(87, 37);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(27, 13);
			this->label11->TabIndex = 68;
			this->label11->Text = L"xi_B";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(184, 37);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(13, 13);
			this->label4->TabIndex = 68;
			this->label4->Text = L"=";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(206, 37);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(48, 13);
			this->label5->TabIndex = 68;
			this->label5->Text = L"maxwGa";
			// 
			// txt_maxwGa
			// 
			this->txt_maxwGa->Location = System::Drawing::Point(263, 34);
			this->txt_maxwGa->Name = L"txt_maxwGa";
			this->txt_maxwGa->Size = System::Drawing::Size(46, 20);
			this->txt_maxwGa->TabIndex = 69;
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(315, 37);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(12, 13);
			this->label6->TabIndex = 68;
			this->label6->Text = L"/";
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(330, 37);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(41, 13);
			this->label8->TabIndex = 68;
			this->label8->Text = L"maxwB";
			// 
			// txt_maxwB
			// 
			this->txt_maxwB->Location = System::Drawing::Point(387, 34);
			this->txt_maxwB->Name = L"txt_maxwB";
			this->txt_maxwB->Size = System::Drawing::Size(46, 20);
			this->txt_maxwB->TabIndex = 69;
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(87, 63);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(28, 13);
			this->label9->TabIndex = 68;
			this->label9->Text = L"xi_G";
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(184, 63);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(13, 13);
			this->label10->TabIndex = 68;
			this->label10->Text = L"=";
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Location = System::Drawing::Point(206, 63);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(48, 13);
			this->label12->TabIndex = 68;
			this->label12->Text = L"maxwGa";
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Location = System::Drawing::Point(315, 63);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(12, 13);
			this->label13->TabIndex = 68;
			this->label13->Text = L"/";
			// 
			// label14
			// 
			this->label14->AutoSize = true;
			this->label14->Location = System::Drawing::Point(330, 63);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(42, 13);
			this->label14->TabIndex = 68;
			this->label14->Text = L"maxwG";
			// 
			// txt_xi_G
			// 
			this->txt_xi_G->Location = System::Drawing::Point(122, 60);
			this->txt_xi_G->Name = L"txt_xi_G";
			this->txt_xi_G->Size = System::Drawing::Size(46, 20);
			this->txt_xi_G->TabIndex = 69;
			// 
			// txt_maxwGa1
			// 
			this->txt_maxwGa1->Location = System::Drawing::Point(263, 60);
			this->txt_maxwGa1->Name = L"txt_maxwGa1";
			this->txt_maxwGa1->Size = System::Drawing::Size(46, 20);
			this->txt_maxwGa1->TabIndex = 69;
			// 
			// txt_maxwG
			// 
			this->txt_maxwG->Location = System::Drawing::Point(387, 60);
			this->txt_maxwG->Name = L"txt_maxwG";
			this->txt_maxwG->Size = System::Drawing::Size(46, 20);
			this->txt_maxwG->TabIndex = 69;
			// 
			// label15
			// 
			this->label15->AutoSize = true;
			this->label15->Location = System::Drawing::Point(87, 89);
			this->label15->Name = L"label15";
			this->label15->Size = System::Drawing::Size(28, 13);
			this->label15->TabIndex = 68;
			this->label15->Text = L"xi_R";
			// 
			// label16
			// 
			this->label16->AutoSize = true;
			this->label16->Location = System::Drawing::Point(184, 89);
			this->label16->Name = L"label16";
			this->label16->Size = System::Drawing::Size(13, 13);
			this->label16->TabIndex = 68;
			this->label16->Text = L"=";
			// 
			// label17
			// 
			this->label17->AutoSize = true;
			this->label17->Location = System::Drawing::Point(206, 89);
			this->label17->Name = L"label17";
			this->label17->Size = System::Drawing::Size(48, 13);
			this->label17->TabIndex = 68;
			this->label17->Text = L"maxwGa";
			// 
			// label18
			// 
			this->label18->AutoSize = true;
			this->label18->Location = System::Drawing::Point(315, 89);
			this->label18->Name = L"label18";
			this->label18->Size = System::Drawing::Size(12, 13);
			this->label18->TabIndex = 68;
			this->label18->Text = L"/";
			// 
			// label19
			// 
			this->label19->AutoSize = true;
			this->label19->Location = System::Drawing::Point(330, 89);
			this->label19->Name = L"label19";
			this->label19->Size = System::Drawing::Size(42, 13);
			this->label19->TabIndex = 68;
			this->label19->Text = L"maxwR";
			// 
			// txt_xi_R
			// 
			this->txt_xi_R->Location = System::Drawing::Point(122, 86);
			this->txt_xi_R->Name = L"txt_xi_R";
			this->txt_xi_R->Size = System::Drawing::Size(46, 20);
			this->txt_xi_R->TabIndex = 69;
			// 
			// txt_maxwGa2
			// 
			this->txt_maxwGa2->Location = System::Drawing::Point(263, 86);
			this->txt_maxwGa2->Name = L"txt_maxwGa2";
			this->txt_maxwGa2->Size = System::Drawing::Size(46, 20);
			this->txt_maxwGa2->TabIndex = 69;
			// 
			// txt_maxwR
			// 
			this->txt_maxwR->Location = System::Drawing::Point(387, 86);
			this->txt_maxwR->Name = L"txt_maxwR";
			this->txt_maxwR->Size = System::Drawing::Size(46, 20);
			this->txt_maxwR->TabIndex = 69;
			// 
			// label20
			// 
			this->label20->AutoSize = true;
			this->label20->Location = System::Drawing::Point(10, 24);
			this->label20->Name = L"label20";
			this->label20->Size = System::Drawing::Size(27, 13);
			this->label20->TabIndex = 68;
			this->label20->Text = L"xi_B";
			// 
			// label21
			// 
			this->label21->AutoSize = true;
			this->label21->Location = System::Drawing::Point(85, 24);
			this->label21->Name = L"label21";
			this->label21->Size = System::Drawing::Size(13, 13);
			this->label21->TabIndex = 68;
			this->label21->Text = L"=";
			// 
			// label22
			// 
			this->label22->AutoSize = true;
			this->label22->Location = System::Drawing::Point(305, 24);
			this->label22->Name = L"label22";
			this->label22->Size = System::Drawing::Size(48, 13);
			this->label22->TabIndex = 68;
			this->label22->Text = L"maxwGa";
			// 
			// label23
			// 
			this->label23->AutoSize = true;
			this->label23->Location = System::Drawing::Point(409, 24);
			this->label23->Name = L"label23";
			this->label23->Size = System::Drawing::Size(12, 13);
			this->label23->TabIndex = 68;
			this->label23->Text = L"/";
			// 
			// label24
			// 
			this->label24->AutoSize = true;
			this->label24->Location = System::Drawing::Point(422, 24);
			this->label24->Name = L"label24";
			this->label24->Size = System::Drawing::Size(41, 13);
			this->label24->TabIndex = 68;
			this->label24->Text = L"maxwB";
			// 
			// txt_xiB
			// 
			this->txt_xiB->Location = System::Drawing::Point(36, 21);
			this->txt_xiB->Name = L"txt_xiB";
			this->txt_xiB->Size = System::Drawing::Size(43, 20);
			this->txt_xiB->TabIndex = 69;
			// 
			// txt_maxwGa4
			// 
			this->txt_maxwGa4->Location = System::Drawing::Point(352, 21);
			this->txt_maxwGa4->Name = L"txt_maxwGa4";
			this->txt_maxwGa4->Size = System::Drawing::Size(59, 20);
			this->txt_maxwGa4->TabIndex = 69;
			// 
			// txt_maxwB1
			// 
			this->txt_maxwB1->Location = System::Drawing::Point(464, 21);
			this->txt_maxwB1->Name = L"txt_maxwB1";
			this->txt_maxwB1->Size = System::Drawing::Size(55, 20);
			this->txt_maxwB1->TabIndex = 69;
			// 
			// label25
			// 
			this->label25->AutoSize = true;
			this->label25->Location = System::Drawing::Point(286, 24);
			this->label25->Name = L"label25";
			this->label25->Size = System::Drawing::Size(11, 13);
			this->label25->TabIndex = 68;
			this->label25->Text = L"*";
			// 
			// label26
			// 
			this->label26->AutoSize = true;
			this->label26->Location = System::Drawing::Point(104, 24);
			this->label26->Name = L"label26";
			this->label26->Size = System::Drawing::Size(26, 13);
			this->label26->TabIndex = 68;
			this->label26->Text = L"max";
			// 
			// txt_max_mean
			// 
			this->txt_max_mean->Location = System::Drawing::Point(132, 21);
			this->txt_max_mean->Name = L"txt_max_mean";
			this->txt_max_mean->Size = System::Drawing::Size(48, 20);
			this->txt_max_mean->TabIndex = 69;
			// 
			// label27
			// 
			this->label27->AutoSize = true;
			this->label27->Location = System::Drawing::Point(186, 24);
			this->label27->Name = L"label27";
			this->label27->Size = System::Drawing::Size(12, 13);
			this->label27->TabIndex = 68;
			this->label27->Text = L"/";
			// 
			// label28
			// 
			this->label28->AutoSize = true;
			this->label28->Location = System::Drawing::Point(206, 24);
			this->label28->Name = L"label28";
			this->label28->Size = System::Drawing::Size(22, 13);
			this->label28->TabIndex = 68;
			this->label28->Text = L"mB";
			// 
			// txt_meanB_float
			// 
			this->txt_meanB_float->Location = System::Drawing::Point(229, 21);
			this->txt_meanB_float->Name = L"txt_meanB_float";
			this->txt_meanB_float->Size = System::Drawing::Size(46, 20);
			this->txt_meanB_float->TabIndex = 69;
			// 
			// label29
			// 
			this->label29->AutoSize = true;
			this->label29->Location = System::Drawing::Point(97, 24);
			this->label29->Name = L"label29";
			this->label29->Size = System::Drawing::Size(10, 13);
			this->label29->TabIndex = 68;
			this->label29->Text = L"(";
			// 
			// label30
			// 
			this->label30->AutoSize = true;
			this->label30->Location = System::Drawing::Point(297, 22);
			this->label30->Name = L"label30";
			this->label30->Size = System::Drawing::Size(10, 13);
			this->label30->TabIndex = 68;
			this->label30->Text = L"(";
			// 
			// label31
			// 
			this->label31->AutoSize = true;
			this->label31->Location = System::Drawing::Point(520, 22);
			this->label31->Name = L"label31";
			this->label31->Size = System::Drawing::Size(10, 13);
			this->label31->TabIndex = 68;
			this->label31->Text = L")";
			this->label31->Click += gcnew System::EventHandler(this, &frm_ASVD::label31_Click);
			// 
			// label32
			// 
			this->label32->AutoSize = true;
			this->label32->Location = System::Drawing::Point(277, 22);
			this->label32->Name = L"label32";
			this->label32->Size = System::Drawing::Size(10, 13);
			this->label32->TabIndex = 68;
			this->label32->Text = L")";
			this->label32->Click += gcnew System::EventHandler(this, &frm_ASVD::label31_Click);
			// 
			// label33
			// 
			this->label33->AutoSize = true;
			this->label33->Location = System::Drawing::Point(10, 58);
			this->label33->Name = L"label33";
			this->label33->Size = System::Drawing::Size(28, 13);
			this->label33->TabIndex = 68;
			this->label33->Text = L"xi_G";
			// 
			// label34
			// 
			this->label34->AutoSize = true;
			this->label34->Location = System::Drawing::Point(87, 58);
			this->label34->Name = L"label34";
			this->label34->Size = System::Drawing::Size(13, 13);
			this->label34->TabIndex = 68;
			this->label34->Text = L"=";
			// 
			// label35
			// 
			this->label35->AutoSize = true;
			this->label35->Location = System::Drawing::Point(305, 58);
			this->label35->Name = L"label35";
			this->label35->Size = System::Drawing::Size(48, 13);
			this->label35->TabIndex = 68;
			this->label35->Text = L"maxwGa";
			// 
			// label36
			// 
			this->label36->AutoSize = true;
			this->label36->Location = System::Drawing::Point(288, 58);
			this->label36->Name = L"label36";
			this->label36->Size = System::Drawing::Size(11, 13);
			this->label36->TabIndex = 68;
			this->label36->Text = L"*";
			// 
			// label37
			// 
			this->label37->AutoSize = true;
			this->label37->Location = System::Drawing::Point(106, 58);
			this->label37->Name = L"label37";
			this->label37->Size = System::Drawing::Size(26, 13);
			this->label37->TabIndex = 68;
			this->label37->Text = L"max";
			// 
			// label38
			// 
			this->label38->AutoSize = true;
			this->label38->Location = System::Drawing::Point(99, 58);
			this->label38->Name = L"label38";
			this->label38->Size = System::Drawing::Size(10, 13);
			this->label38->TabIndex = 68;
			this->label38->Text = L"(";
			// 
			// label39
			// 
			this->label39->AutoSize = true;
			this->label39->Location = System::Drawing::Point(297, 56);
			this->label39->Name = L"label39";
			this->label39->Size = System::Drawing::Size(10, 13);
			this->label39->TabIndex = 68;
			this->label39->Text = L"(";
			// 
			// label40
			// 
			this->label40->AutoSize = true;
			this->label40->Location = System::Drawing::Point(520, 58);
			this->label40->Name = L"label40";
			this->label40->Size = System::Drawing::Size(10, 13);
			this->label40->TabIndex = 68;
			this->label40->Text = L")";
			this->label40->Click += gcnew System::EventHandler(this, &frm_ASVD::label31_Click);
			// 
			// label41
			// 
			this->label41->AutoSize = true;
			this->label41->Location = System::Drawing::Point(279, 56);
			this->label41->Name = L"label41";
			this->label41->Size = System::Drawing::Size(10, 13);
			this->label41->TabIndex = 68;
			this->label41->Text = L")";
			this->label41->Click += gcnew System::EventHandler(this, &frm_ASVD::label31_Click);
			// 
			// label42
			// 
			this->label42->AutoSize = true;
			this->label42->Location = System::Drawing::Point(206, 58);
			this->label42->Name = L"label42";
			this->label42->Size = System::Drawing::Size(23, 13);
			this->label42->TabIndex = 68;
			this->label42->Text = L"mG";
			// 
			// label43
			// 
			this->label43->AutoSize = true;
			this->label43->Location = System::Drawing::Point(411, 58);
			this->label43->Name = L"label43";
			this->label43->Size = System::Drawing::Size(12, 13);
			this->label43->TabIndex = 68;
			this->label43->Text = L"/";
			// 
			// label44
			// 
			this->label44->AutoSize = true;
			this->label44->Location = System::Drawing::Point(188, 58);
			this->label44->Name = L"label44";
			this->label44->Size = System::Drawing::Size(12, 13);
			this->label44->TabIndex = 68;
			this->label44->Text = L"/";
			// 
			// label45
			// 
			this->label45->AutoSize = true;
			this->label45->Location = System::Drawing::Point(422, 58);
			this->label45->Name = L"label45";
			this->label45->Size = System::Drawing::Size(42, 13);
			this->label45->TabIndex = 68;
			this->label45->Text = L"maxwG";
			// 
			// txt_xiG
			// 
			this->txt_xiG->Location = System::Drawing::Point(38, 55);
			this->txt_xiG->Name = L"txt_xiG";
			this->txt_xiG->Size = System::Drawing::Size(43, 20);
			this->txt_xiG->TabIndex = 69;
			// 
			// txt_maxwGa5
			// 
			this->txt_maxwGa5->Location = System::Drawing::Point(352, 55);
			this->txt_maxwGa5->Name = L"txt_maxwGa5";
			this->txt_maxwGa5->Size = System::Drawing::Size(59, 20);
			this->txt_maxwGa5->TabIndex = 69;
			this->txt_maxwGa5->TextChanged += gcnew System::EventHandler(this, &frm_ASVD::textBox7_TextChanged);
			// 
			// txt_max_mean1
			// 
			this->txt_max_mean1->Location = System::Drawing::Point(134, 55);
			this->txt_max_mean1->Name = L"txt_max_mean1";
			this->txt_max_mean1->Size = System::Drawing::Size(48, 20);
			this->txt_max_mean1->TabIndex = 69;
			this->txt_max_mean1->TextChanged += gcnew System::EventHandler(this, &frm_ASVD::textBox8_TextChanged);
			// 
			// txt_meanG_float
			// 
			this->txt_meanG_float->Location = System::Drawing::Point(231, 55);
			this->txt_meanG_float->Name = L"txt_meanG_float";
			this->txt_meanG_float->Size = System::Drawing::Size(46, 20);
			this->txt_meanG_float->TabIndex = 69;
			// 
			// txt_maxwG1
			// 
			this->txt_maxwG1->Location = System::Drawing::Point(464, 55);
			this->txt_maxwG1->Name = L"txt_maxwG1";
			this->txt_maxwG1->Size = System::Drawing::Size(55, 20);
			this->txt_maxwG1->TabIndex = 69;
			// 
			// label46
			// 
			this->label46->AutoSize = true;
			this->label46->Location = System::Drawing::Point(10, 92);
			this->label46->Name = L"label46";
			this->label46->Size = System::Drawing::Size(28, 13);
			this->label46->TabIndex = 68;
			this->label46->Text = L"xi_R";
			// 
			// label47
			// 
			this->label47->AutoSize = true;
			this->label47->Location = System::Drawing::Point(85, 92);
			this->label47->Name = L"label47";
			this->label47->Size = System::Drawing::Size(13, 13);
			this->label47->TabIndex = 68;
			this->label47->Text = L"=";
			// 
			// label48
			// 
			this->label48->AutoSize = true;
			this->label48->Location = System::Drawing::Point(305, 92);
			this->label48->Name = L"label48";
			this->label48->Size = System::Drawing::Size(48, 13);
			this->label48->TabIndex = 68;
			this->label48->Text = L"maxwGa";
			// 
			// label49
			// 
			this->label49->AutoSize = true;
			this->label49->Location = System::Drawing::Point(286, 92);
			this->label49->Name = L"label49";
			this->label49->Size = System::Drawing::Size(11, 13);
			this->label49->TabIndex = 68;
			this->label49->Text = L"*";
			// 
			// label50
			// 
			this->label50->AutoSize = true;
			this->label50->Location = System::Drawing::Point(104, 92);
			this->label50->Name = L"label50";
			this->label50->Size = System::Drawing::Size(26, 13);
			this->label50->TabIndex = 68;
			this->label50->Text = L"max";
			// 
			// label51
			// 
			this->label51->AutoSize = true;
			this->label51->Location = System::Drawing::Point(97, 92);
			this->label51->Name = L"label51";
			this->label51->Size = System::Drawing::Size(10, 13);
			this->label51->TabIndex = 68;
			this->label51->Text = L"(";
			// 
			// label52
			// 
			this->label52->AutoSize = true;
			this->label52->Location = System::Drawing::Point(297, 90);
			this->label52->Name = L"label52";
			this->label52->Size = System::Drawing::Size(10, 13);
			this->label52->TabIndex = 68;
			this->label52->Text = L"(";
			// 
			// label53
			// 
			this->label53->AutoSize = true;
			this->label53->Location = System::Drawing::Point(520, 90);
			this->label53->Name = L"label53";
			this->label53->Size = System::Drawing::Size(10, 13);
			this->label53->TabIndex = 68;
			this->label53->Text = L")";
			this->label53->Click += gcnew System::EventHandler(this, &frm_ASVD::label31_Click);
			// 
			// label54
			// 
			this->label54->AutoSize = true;
			this->label54->Location = System::Drawing::Point(277, 90);
			this->label54->Name = L"label54";
			this->label54->Size = System::Drawing::Size(10, 13);
			this->label54->TabIndex = 68;
			this->label54->Text = L")";
			this->label54->Click += gcnew System::EventHandler(this, &frm_ASVD::label31_Click);
			// 
			// label55
			// 
			this->label55->AutoSize = true;
			this->label55->Location = System::Drawing::Point(206, 92);
			this->label55->Name = L"label55";
			this->label55->Size = System::Drawing::Size(23, 13);
			this->label55->TabIndex = 68;
			this->label55->Text = L"mR";
			// 
			// label56
			// 
			this->label56->AutoSize = true;
			this->label56->Location = System::Drawing::Point(409, 92);
			this->label56->Name = L"label56";
			this->label56->Size = System::Drawing::Size(12, 13);
			this->label56->TabIndex = 68;
			this->label56->Text = L"/";
			// 
			// label57
			// 
			this->label57->AutoSize = true;
			this->label57->Location = System::Drawing::Point(186, 92);
			this->label57->Name = L"label57";
			this->label57->Size = System::Drawing::Size(12, 13);
			this->label57->TabIndex = 68;
			this->label57->Text = L"/";
			// 
			// label58
			// 
			this->label58->AutoSize = true;
			this->label58->Location = System::Drawing::Point(422, 92);
			this->label58->Name = L"label58";
			this->label58->Size = System::Drawing::Size(42, 13);
			this->label58->TabIndex = 68;
			this->label58->Text = L"maxwR";
			// 
			// txt_xiR
			// 
			this->txt_xiR->Location = System::Drawing::Point(36, 89);
			this->txt_xiR->Name = L"txt_xiR";
			this->txt_xiR->Size = System::Drawing::Size(43, 20);
			this->txt_xiR->TabIndex = 69;
			// 
			// txt_maxwGa6
			// 
			this->txt_maxwGa6->Location = System::Drawing::Point(352, 89);
			this->txt_maxwGa6->Name = L"txt_maxwGa6";
			this->txt_maxwGa6->Size = System::Drawing::Size(59, 20);
			this->txt_maxwGa6->TabIndex = 69;
			// 
			// txt_max_mean2
			// 
			this->txt_max_mean2->Location = System::Drawing::Point(132, 89);
			this->txt_max_mean2->Name = L"txt_max_mean2";
			this->txt_max_mean2->Size = System::Drawing::Size(48, 20);
			this->txt_max_mean2->TabIndex = 69;
			this->txt_max_mean2->TextChanged += gcnew System::EventHandler(this, &frm_ASVD::textBox13_TextChanged);
			// 
			// txt_meanR_float
			// 
			this->txt_meanR_float->Location = System::Drawing::Point(229, 89);
			this->txt_meanR_float->Name = L"txt_meanR_float";
			this->txt_meanR_float->Size = System::Drawing::Size(46, 20);
			this->txt_meanR_float->TabIndex = 69;
			// 
			// txt_maxwR1
			// 
			this->txt_maxwR1->Location = System::Drawing::Point(464, 89);
			this->txt_maxwR1->Name = L"txt_maxwR1";
			this->txt_maxwR1->Size = System::Drawing::Size(55, 20);
			this->txt_maxwR1->TabIndex = 69;
			// 
			// ASVD
			// 
			this->ASVD->Controls->Add(this->txt_maxwR1);
			this->ASVD->Controls->Add(this->txt_maxwG1);
			this->ASVD->Controls->Add(this->txt_maxwB1);
			this->ASVD->Controls->Add(this->txt_meanR_float);
			this->ASVD->Controls->Add(this->txt_meanG_float);
			this->ASVD->Controls->Add(this->txt_meanB_float);
			this->ASVD->Controls->Add(this->txt_max_mean2);
			this->ASVD->Controls->Add(this->txt_max_mean1);
			this->ASVD->Controls->Add(this->txt_max_mean);
			this->ASVD->Controls->Add(this->txt_maxwGa6);
			this->ASVD->Controls->Add(this->txt_maxwGa5);
			this->ASVD->Controls->Add(this->txt_maxwGa4);
			this->ASVD->Controls->Add(this->txt_xiR);
			this->ASVD->Controls->Add(this->txt_xiG);
			this->ASVD->Controls->Add(this->txt_xiB);
			this->ASVD->Controls->Add(this->label58);
			this->ASVD->Controls->Add(this->label45);
			this->ASVD->Controls->Add(this->label24);
			this->ASVD->Controls->Add(this->label57);
			this->ASVD->Controls->Add(this->label40);
			this->ASVD->Controls->Add(this->label44);
			this->ASVD->Controls->Add(this->label27);
			this->ASVD->Controls->Add(this->label56);
			this->ASVD->Controls->Add(this->label43);
			this->ASVD->Controls->Add(this->label23);
			this->ASVD->Controls->Add(this->label55);
			this->ASVD->Controls->Add(this->label42);
			this->ASVD->Controls->Add(this->label28);
			this->ASVD->Controls->Add(this->label54);
			this->ASVD->Controls->Add(this->label41);
			this->ASVD->Controls->Add(this->label32);
			this->ASVD->Controls->Add(this->label53);
			this->ASVD->Controls->Add(this->label31);
			this->ASVD->Controls->Add(this->label52);
			this->ASVD->Controls->Add(this->label39);
			this->ASVD->Controls->Add(this->label30);
			this->ASVD->Controls->Add(this->label51);
			this->ASVD->Controls->Add(this->label38);
			this->ASVD->Controls->Add(this->label29);
			this->ASVD->Controls->Add(this->label50);
			this->ASVD->Controls->Add(this->label37);
			this->ASVD->Controls->Add(this->label26);
			this->ASVD->Controls->Add(this->label49);
			this->ASVD->Controls->Add(this->label36);
			this->ASVD->Controls->Add(this->label25);
			this->ASVD->Controls->Add(this->label48);
			this->ASVD->Controls->Add(this->label35);
			this->ASVD->Controls->Add(this->label22);
			this->ASVD->Controls->Add(this->label47);
			this->ASVD->Controls->Add(this->label34);
			this->ASVD->Controls->Add(this->label46);
			this->ASVD->Controls->Add(this->label21);
			this->ASVD->Controls->Add(this->label33);
			this->ASVD->Controls->Add(this->label20);
			this->ASVD->Location = System::Drawing::Point(12, 320);
			this->ASVD->Name = L"ASVD";
			this->ASVD->Size = System::Drawing::Size(538, 125);
			this->ASVD->TabIndex = 70;
			this->ASVD->TabStop = false;
			this->ASVD->Text = L"ASVD Parameters";
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->txt_maxwR);
			this->groupBox1->Controls->Add(this->txt_maxwG);
			this->groupBox1->Controls->Add(this->txt_maxwGa2);
			this->groupBox1->Controls->Add(this->txt_maxwGa1);
			this->groupBox1->Controls->Add(this->txt_maxwB);
			this->groupBox1->Controls->Add(this->txt_xi_R);
			this->groupBox1->Controls->Add(this->txt_xi_G);
			this->groupBox1->Controls->Add(this->label19);
			this->groupBox1->Controls->Add(this->txt_maxwGa);
			this->groupBox1->Controls->Add(this->label14);
			this->groupBox1->Controls->Add(this->label18);
			this->groupBox1->Controls->Add(this->txt_xi_B);
			this->groupBox1->Controls->Add(this->label13);
			this->groupBox1->Controls->Add(this->label17);
			this->groupBox1->Controls->Add(this->label8);
			this->groupBox1->Controls->Add(this->label12);
			this->groupBox1->Controls->Add(this->label16);
			this->groupBox1->Controls->Add(this->label6);
			this->groupBox1->Controls->Add(this->label10);
			this->groupBox1->Controls->Add(this->label15);
			this->groupBox1->Controls->Add(this->label5);
			this->groupBox1->Controls->Add(this->label9);
			this->groupBox1->Controls->Add(this->label4);
			this->groupBox1->Controls->Add(this->label11);
			this->groupBox1->Location = System::Drawing::Point(12, 183);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(538, 131);
			this->groupBox1->TabIndex = 71;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"R&D parameters";
			// 
			// panel1
			// 
			this->panel1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->panel1->Controls->Add(this->nUD_sigma);
			this->panel1->Controls->Add(this->nUD_mGa);
			this->panel1->Controls->Add(this->label7);
			this->panel1->Controls->Add(this->label3);
			this->panel1->Controls->Add(this->btn_PCS);
			this->panel1->Controls->Add(this->btn_RD);
			this->panel1->Controls->Add(this->btn_ASVD);
			this->panel1->Location = System::Drawing::Point(12, 61);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(538, 111);
			this->panel1->TabIndex = 72;
			// 
			// frm_ASVD
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1187, 482);
			this->Controls->Add(this->panel1);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->lab_result);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->lab_ori);
			this->Controls->Add(this->pB_hist_dst);
			this->Controls->Add(this->pB_hist_src);
			this->Controls->Add(this->pB_ASVD);
			this->Controls->Add(this->pB_ori);
			this->Controls->Add(this->ASVD);
			this->Name = L"frm_ASVD";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Adaptive Singular Value Decomposition";
			this->Load += gcnew System::EventHandler(this, &frm_ASVD::frm_ASVD_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_hist_dst))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_hist_src))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_ASVD))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_ori))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUD_sigma))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUD_mGa))->EndInit();
			this->ASVD->ResumeLayout(false);
			this->ASVD->PerformLayout();
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->panel1->ResumeLayout(false);
			this->panel1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void btn_RD_Click(System::Object^  sender, System::EventArgs^  e)
			 {
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
				
					
					char* imgname;
					imgname = (char*)Marshal::StringToHGlobalAnsi(OpenFileName).ToPointer();  // convert to kieu char lay ten filecho opencv
					Mat img = cv::imread(imgname);
				

					int m = img.rows;
					int n = img.cols;
					

					Mat B = Mat_<float>(m,n);
					Mat G = Mat_<float>(m,n);
					Mat R = Mat_<float>(m,n);
					Mat img_src;
	
					img.convertTo(img_src, CV_32F);

					for (int i = 0;i<img.cols;i++)
					{
						for (int j =0; j<img.rows;j++)
						{
							B.at<float>(cv::Point(i, j)) = img_src.at<Vec3f>(cv::Point(i, j))[0];
							G.at<float>(cv::Point(i, j)) = img_src.at<Vec3f>(cv::Point(i, j))[1];
							R.at<float>(cv::Point(i, j)) = img_src.at<Vec3f>(cv::Point(i, j))[2];
						}
					}
					
		
					
					// Synthetic Gaussian intensity maxtrix
					float mi_Ga = (float)nUD_mGa->Value;
					float sigma_Ga = (float)nUD_sigma->Value;
					Mat Ga = Mat(m, n, CV_32F);
					randn(Ga, mi_Ga, sigma_Ga);
				
					// Compute SVD of Gaussian Matrix
					Mat wGa, uGa,vtGa;
					SVD::compute(Ga,wGa, uGa, vtGa, SVD::NO_UV);
					double minwGa, maxwGa;
					minMaxLoc(wGa, &minwGa, &maxwGa);
					
					// Compute SVD of each channels
					// SVD of Blue channel
					SVD svd(B);
					double minwB, maxwB;
					minMaxLoc(svd.w, &minwB, &maxwB);
					
					
					float xiB = (float)(maxwGa/maxwB);
					
					for (int i = 0;i<svd.w.cols;i++)
					{
						for (int j =0; j<svd.w.rows;j++)
						{
							svd.w.at<float>(cv::Point(i, j))= xiB*(svd.w.at<float>(cv::Point(i, j)));
						}
					}
				
					Mat B_svd = svd.u*Mat::diag(svd.w)*svd.vt;
					// SVD of Green channel
					Mat wG, uG, vtG;
					SVD::compute(G,wG,uG,vtG,SVD::FULL_UV);
				
					double minwG, maxwG;
					minMaxLoc(wG, &minwG, &maxwG);
					
					float xiG = (float)(maxwGa/maxwG);
					
					for (int i = 0;i<svd.w.cols;i++)
					{
						for (int j =0; j<svd.w.rows;j++)
						{
							svd.w.at<float>(cv::Point(i, j))= xiB*(wG.at<float>(cv::Point(i, j)));
						}
					}
					for (int i = 0;i<svd.u.cols;i++)
					{
						for (int j =0; j<svd.u.rows;j++)
						{
							svd.u.at<float>(cv::Point(i, j))= uG.at<float>(cv::Point(i, j));
						}
					}
					for (int i = 0;i<svd.vt.cols;i++)
					{
						for (int j =0; j<svd.vt.rows;j++)
						{
							svd.vt.at<float>(cv::Point(i, j))= vtG.at<float>(cv::Point(i, j));
						}
					}
					Mat G_svd = svd.u*Mat::diag(svd.w)*svd.vt;
					// SVD of Red channel
					Mat wR, uR, vtR;
					SVD::compute(R,wR,uR,vtR,SVD::FULL_UV);
				
					double minwR, maxwR;
					minMaxLoc(wR, &minwR, &maxwR);
					
					
					float xiR = (float)(maxwGa/maxwR);
					
					for (int i = 0;i<svd.w.cols;i++)
					{
						for (int j =0; j<svd.w.rows;j++)
						{
							svd.w.at<float>(cv::Point(i, j))= xiB*(wR.at<float>(cv::Point(i, j)));
						}
					}
					for (int i = 0;i<svd.u.cols;i++)
					{
						for (int j =0; j<svd.u.rows;j++)
						{
						svd.u.at<float>(cv::Point(i, j))= uR.at<float>(cv::Point(i, j));
						}
					}
					for (int i = 0;i<svd.vt.cols;i++)
					{
						for (int j =0; j<svd.vt.rows;j++)
						{
							svd.vt.at<float>(cv::Point(i, j))= vtR.at<float>(cv::Point(i, j));
						}
					}
					Mat R_svd = svd.u*Mat::diag(svd.w)*svd.vt;
					// SVD Image
					Mat img_svd = img_src;
				
					for (int i = 0;i<img_src.cols;i++)
					{
						for (int j =0; j<img_src.rows;j++)
						{
							
							img_svd.at<Vec3f>(cv::Point(i, j))[0] = B_svd.at<float>(cv::Point(i, j));
							img_svd.at<Vec3f>(cv::Point(i, j))[1] = G_svd.at<float>(cv::Point(i, j));
							img_svd.at<Vec3f>(cv::Point(i, j))[2] = R_svd.at<float>(cv::Point(i, j));
							
						}
					}
					

					normalize(img_svd,img_svd,0,255,NORM_MINMAX, CV_8UC3);
					img_svd.convertTo(img_svd, CV_8UC3);
					char* savename;
					savename = (char*)Marshal::StringToHGlobalAnsi("..\\results\\RD\\" + (step+1).ToString()+".bmp").ToPointer();
					imwrite( savename, img_svd );
					
					txt_maxwGa->Text = maxwGa.ToString(); txt_maxwGa->Refresh();
					txt_maxwGa1->Text = maxwGa.ToString();txt_maxwGa1->Refresh();
					txt_maxwGa2->Text = maxwGa.ToString();txt_maxwGa2->Refresh();
					txt_maxwB->Text = maxwB.ToString();txt_maxwB->Refresh();
					txt_maxwG->Text = maxwG.ToString();txt_maxwG->Refresh();
					txt_maxwR->Text = maxwR.ToString();txt_maxwR->Refresh();
					txt_xi_B->Text = xiB.ToString();txt_xi_B->Refresh();
					txt_xi_G->Text = xiG.ToString();txt_xi_G->Refresh();
					txt_xi_R->Text = xiR.ToString();txt_xi_R->Refresh();
					/*lstBox->Items->Add("xi factor of Blue channel: " + xiB.ToString());
					lstBox->Items->Add("maxwG: " + maxwG.ToString());
					lstBox->Items->Add("xi factor of Green channel: " + xiG.ToString());
					lstBox->Items->Add("maxwR: " + maxwR.ToString());
					lstBox->Items->Add("xi factor of Red channel: " + xiR.ToString());*/
					

					Mat hist = drawhistogram(img);
					Mat hist_dst = drawhistogram(img_svd);
					

					
				
					
					/// Display

					int row_s = (img.rows/4)*4;
					int col_s = (img.cols/4)*4;

					Mat img_s = Mat(row_s, col_s, img.type());
					resize(img, img_s, img_s.size(), 0, 0, INTER_LINEAR);
					pB_ori->Image  = gcnew    
					System::Drawing::Bitmap(img_s.size().width,img_s.size().height,img_s.step,
					System::Drawing::Imaging::PixelFormat::Format24bppRgb,(System::IntPtr) img_s.data);
					pB_ori->Refresh();

					Mat img_svd_s = Mat(row_s, col_s, img.type());
					resize(img_svd, img_svd_s, img_svd_s.size(), 0, 0, INTER_LINEAR);

					pB_ASVD->Image  = gcnew    
					System::Drawing::Bitmap(img_svd_s.size().width,img_svd_s.size().height,img_svd_s.step,
					System::Drawing::Imaging::PixelFormat::Format24bppRgb,(System::IntPtr) img_svd_s.data);
					pB_ASVD->Refresh();
					
					pB_hist_src->Image  = gcnew    
					System::Drawing::Bitmap(hist.size().width,hist.size().height,hist.step,
					System::Drawing::Imaging::PixelFormat::Format24bppRgb,(System::IntPtr) hist.data);
					pB_hist_src->Refresh();

					pB_hist_dst->Image  = gcnew    
					System::Drawing::Bitmap(hist_dst.size().width,hist_dst.size().height,hist_dst.step,
					System::Drawing::Imaging::PixelFormat::Format24bppRgb,(System::IntPtr) hist_dst.data);
					pB_hist_dst->Refresh();
					
					step+=1;
				}
			 }
	private: System::Void frm_ASVD_Load(System::Object^  sender, System::EventArgs^  e)
			 {
				 nUD_mGa->Value = 127;
				 nUD_sigma->Value = 32;
				
			 }
	
private: System::Void btn_ASVD_Click(System::Object^  sender, System::EventArgs^  e)
		 {
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
				
					
					char* imgname;
					imgname = (char*)Marshal::StringToHGlobalAnsi(OpenFileName).ToPointer();  // convert to kieu char lay ten filecho opencv
					Mat img = cv::imread(imgname);
				

					int m = img.rows;
					int n = img.cols;
					

					Mat B = Mat_<float>(m,n);
					Mat G = Mat_<float>(m,n);
					Mat R = Mat_<float>(m,n);
					Mat img_src;
	
					img.convertTo(img_src, CV_32F);
					for (int i = 0;i<img.cols;i++)
					{
						for (int j =0; j<img.rows;j++)
						{
							
							B.at<float>(cv::Point(i, j)) = img_src.at<Vec3f>(cv::Point(i, j))[0];
							G.at<float>(cv::Point(i, j)) = img_src.at<Vec3f>(cv::Point(i, j))[1];
							R.at<float>(cv::Point(i, j)) = img_src.at<Vec3f>(cv::Point(i, j))[2];
							//lstBox->Items->Add(B.at<float>(cv::Point(i, j)).ToString());
							
						}
					}
					// mean of Blue channel
					Scalar meanB, stddevB;
					meanStdDev(B, meanB, stddevB);
					float meanB_float= (float)meanB.val[0];
					
					// mean of Green channel
					Scalar meanG, stddevG;
					meanStdDev(G, meanG, stddevG);
					float meanG_float= (float)meanG.val[0];
					
					// mean of Red channel
					Scalar meanR, stddevR;
					meanStdDev(R, meanR, stddevR);
					float meanR_float= (float)meanR.val[0];
					
					//max of mean's channels
					float max1 = meanB_float >= meanG_float?meanB_float:meanG_float;
					float max_mean = max1 >= meanR_float?max1:meanR_float;
					//float max_mean = (meanB_float + meanG_float + meanR_float)/3.0;
					//float min1 = meanB_float <= meanG_float?meanB_float:meanG_float;
					//float max_mean = min1 <= meanR_float?min1:meanR_float;
					
		
					
					// Synthetic Gaussian intensity maxtrix
					Mat Ga = Mat(m, n, CV_32F);
					randn(Ga, 128.0, 32.0);
				
					// Compute SVD of Gaussian Matrix
					Mat wGa, uGa,vtGa;
					SVD::compute(Ga,wGa, uGa, vtGa, SVD::NO_UV);
					double minwGa, maxwGa;
					minMaxLoc(wGa, &minwGa, &maxwGa);
					
					// Compute SVD of each channels
					// SVD of Blue channel
					SVD svd(B);
					double minwB, maxwB;
					minMaxLoc(svd.w, &minwB, &maxwB);
					
					//float xiB = (float)(max_mean/meanB_float)*(maxwGa/maxwB);
					float xiB = (float)(meanB_float/max_mean)*(maxwGa/maxwB);
					

					for (int i = 0;i<svd.w.cols;i++)
					{
						for (int j =0; j<svd.w.rows;j++)
						{
							svd.w.at<float>(cv::Point(i, j))= xiB*(svd.w.at<float>(cv::Point(i, j)));
						}
					}
				
					Mat B_svd = svd.u*Mat::diag(svd.w)*svd.vt;
					// SVD of Green channel
					Mat wG, uG, vtG;
					SVD::compute(G,wG,uG,vtG,SVD::FULL_UV);
				
					double minwG, maxwG;
					minMaxLoc(wG, &minwG, &maxwG);
					

					//float xiG = (float)(max_mean/meanG_float)*(maxwGa/maxwG);
					float xiG = (float)(meanG_float/max_mean)*(maxwGa/maxwG);
					
					for (int i = 0;i<svd.w.cols;i++)
					{
						for (int j =0; j<svd.w.rows;j++)
						{
							svd.w.at<float>(cv::Point(i, j))= xiG*(wG.at<float>(cv::Point(i, j)));
						}
					}
					for (int i = 0;i<svd.u.cols;i++)
					{
						for (int j =0; j<svd.u.rows;j++)
						{
							svd.u.at<float>(cv::Point(i, j))= uG.at<float>(cv::Point(i, j));
						}
					}
					for (int i = 0;i<svd.vt.cols;i++)
					{
						for (int j =0; j<svd.vt.rows;j++)
						{
							svd.vt.at<float>(cv::Point(i, j))= vtG.at<float>(cv::Point(i, j));
						}
					}
					Mat G_svd = svd.u*Mat::diag(svd.w)*svd.vt;
					// SVD of Red channel
					Mat wR, uR, vtR;
					SVD::compute(R,wR,uR,vtR,SVD::FULL_UV);
				
					double minwR, maxwR;
					minMaxLoc(wR, &minwR, &maxwR);
					
					//float xiR = (float)(max_mean/meanR_float)*(maxwGa/maxwR);
					float xiR = (float)(meanR_float/max_mean)*(maxwGa/maxwR);

					for (int i = 0;i<svd.w.cols;i++)
					{
						for (int j =0; j<svd.w.rows;j++)
						{
							svd.w.at<float>(cv::Point(i, j))= xiR*(wR.at<float>(cv::Point(i, j)));
						}
					}
					for (int i = 0;i<svd.u.cols;i++)
					{
						for (int j =0; j<svd.u.rows;j++)
						{
						svd.u.at<float>(cv::Point(i, j))= uR.at<float>(cv::Point(i, j));
						}
					}
					for (int i = 0;i<svd.vt.cols;i++)
					{
						for (int j =0; j<svd.vt.rows;j++)
						{
							svd.vt.at<float>(cv::Point(i, j))= vtR.at<float>(cv::Point(i, j));
						}
					}
					Mat R_svd = svd.u*Mat::diag(svd.w)*svd.vt;
					// SVD Image
					Mat img_svd = img_src;
				
					for (int i = 0;i<img_src.cols;i++)
					{
						for (int j =0; j<img_src.rows;j++)
						{
							
							img_svd.at<Vec3f>(cv::Point(i, j))[0] = B_svd.at<float>(cv::Point(i, j));
							img_svd.at<Vec3f>(cv::Point(i, j))[1] = G_svd.at<float>(cv::Point(i, j));
							img_svd.at<Vec3f>(cv::Point(i, j))[2] = R_svd.at<float>(cv::Point(i, j));
							//lstBox->Items->Add(B.at<float>(cv::Point(i, j)).ToString());
							
						}
					}
					
					

					txt_meanG_float->Text = meanG_float.ToString();txt_meanG_float->Refresh();
					txt_meanB_float->Text = meanB_float.ToString();txt_meanB_float->Refresh();
					txt_meanR_float->Text = meanR_float.ToString();txt_meanR_float->Refresh();
					txt_max_mean->Text = max_mean.ToString();txt_max_mean->Refresh();
					txt_max_mean1->Text = max_mean.ToString();txt_max_mean1->Refresh();
					txt_max_mean2->Text = max_mean.ToString();txt_max_mean2->Refresh();
					txt_maxwGa4->Text = maxwGa.ToString();txt_maxwGa4->Refresh();
					txt_maxwGa5->Text = maxwGa.ToString();txt_maxwGa5->Refresh();
					txt_maxwGa6->Text = maxwGa.ToString();txt_maxwGa6->Refresh();
					txt_maxwB1->Text = maxwB.ToString();txt_maxwB1->Refresh();
					txt_maxwG1->Text = maxwG.ToString();txt_maxwG1->Refresh();
					txt_maxwR1->Text = maxwR.ToString();txt_maxwR1->Refresh();
					txt_xiB->Text = xiB.ToString();txt_xiB->Refresh();
					txt_xiG->Text = xiG.ToString();txt_xiG->Refresh();
					txt_xiR->Text = xiR.ToString();txt_xiR->Refresh();

					normalize(img_svd, img_svd, 0, 255, NORM_MINMAX, CV_8UC3);
					img_svd.convertTo(img_svd, CV_8UC3);
					char* savename;
					savename = (char*)Marshal::StringToHGlobalAnsi("..\\results\\ASVD\\" + (step+1).ToString()+".bmp").ToPointer();
					imwrite( savename, img_svd );
					

					

					Mat hist = drawhistogram(img);
					Mat hist_dst = drawhistogram(img_svd);
					

					
				
					
					/// Display

					int row_s = (img.rows/4)*4;
					int col_s = (img.cols/4)*4;

					Mat img_s = Mat(row_s, col_s, img.type());
					resize(img, img_s, img_s.size(), 0, 0, INTER_LINEAR);
					pB_ori->Image  = gcnew    
					System::Drawing::Bitmap(img_s.size().width,img_s.size().height,img_s.step,
					System::Drawing::Imaging::PixelFormat::Format24bppRgb,(System::IntPtr) img_s.data);
					pB_ori->Refresh();

					Mat img_svd_s = Mat(row_s, col_s, img.type());
					resize(img_svd, img_svd_s, img_svd_s.size(), 0, 0, INTER_LINEAR);

					pB_ASVD->Image  = gcnew    
					System::Drawing::Bitmap(img_svd_s.size().width,img_svd_s.size().height,img_svd_s.step,
					System::Drawing::Imaging::PixelFormat::Format24bppRgb,(System::IntPtr) img_svd_s.data);
					pB_ASVD->Refresh();
					
					pB_hist_src->Image  = gcnew    
					System::Drawing::Bitmap(hist.size().width,hist.size().height,hist.step,
					System::Drawing::Imaging::PixelFormat::Format24bppRgb,(System::IntPtr) hist.data);
					pB_hist_src->Refresh();

					pB_hist_dst->Image  = gcnew    
					System::Drawing::Bitmap(hist_dst.size().width,hist_dst.size().height,hist_dst.step,
					System::Drawing::Imaging::PixelFormat::Format24bppRgb,(System::IntPtr) hist_dst.data);
					pB_hist_dst->Refresh();
					
					step+=1;
				}

		 }
private: System::Void btn_PCS_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
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
				
					
					char* imgname;
					imgname = (char*)Marshal::StringToHGlobalAnsi(OpenFileName).ToPointer();  // convert to kieu char lay ten filecho opencv
					Mat img = cv::imread(imgname);
				

					int m = img.rows;
					int n = img.cols;
					

					Mat B = Mat_<float>(m,n);
					Mat G = Mat_<float>(m,n);
					Mat R = Mat_<float>(m,n);
					Mat img_src;
	
					img.convertTo(img_src, CV_32F);
					for (int i = 0;i<img.cols;i++)
					{
						for (int j =0; j<img.rows;j++)
						{
							
							B.at<float>(cv::Point(i, j)) = img_src.at<Vec3f>(cv::Point(i, j))[0];
							G.at<float>(cv::Point(i, j)) = img_src.at<Vec3f>(cv::Point(i, j))[1];
							R.at<float>(cv::Point(i, j)) = img_src.at<Vec3f>(cv::Point(i, j))[2];
							//lstBox->Items->Add(B.at<float>(cv::Point(i, j)).ToString());
							
						}
					}
					//PCS
					Mat temp = Mat_<float>(m,m);
					Mat Gt = Mat_<float>(n,m);
					cv::transpose(G,Gt);
					temp = R*Gt;
					Mat PCS1 = Mat_<float>(m,n);
					PCS1 = temp*B;
					Mat PCS = Mat::zeros(m,n,img_src.type());
					for (int i = 0;i<img.cols;i++)
					{
						for (int j =0; j<img.rows;j++)
						{
							
							PCS.at<Vec3f>(cv::Point(i, j))[0] = PCS1.at<float>(cv::Point(i, j));
							PCS.at<Vec3f>(cv::Point(i, j))[1] = PCS1.at<float>(cv::Point(i, j));
							PCS.at<Vec3f>(cv::Point(i, j))[2] = PCS1.at<float>(cv::Point(i, j));
							
						}
					}
					normalize(PCS,PCS,0,255,NORM_MINMAX, CV_8UC3);
					PCS.convertTo(PCS, CV_8UC3);
					
					char* savename;
					savename = (char*)Marshal::StringToHGlobalAnsi("..\\results\\PCS\\" + (step+1).ToString()+".bmp").ToPointer();
					imwrite( savename, PCS );
					

					

					Mat hist = drawhistogram(img);
					Mat hist_dst = drawhistogram(PCS);
					int row_s = (img.rows/4)*4;
					int col_s = (img.cols/4)*4;
					Mat img_s = Mat(row_s, col_s, img.type());
					resize(img, img_s, img_s.size(), 0, 0, INTER_LINEAR);
					pB_ori->Image  = gcnew    
					System::Drawing::Bitmap(img_s.size().width,img_s.size().height,img_s.step,
					System::Drawing::Imaging::PixelFormat::Format24bppRgb,(System::IntPtr) img_s.data);
					pB_ori->Refresh();

					Mat PCS_s = Mat(row_s, col_s, img.type());
					resize(PCS, PCS_s, PCS_s.size(), 0, 0, INTER_LINEAR);

					pB_ASVD->Image  = gcnew    
					System::Drawing::Bitmap(PCS_s.size().width,PCS_s.size().height,PCS_s.step,
					System::Drawing::Imaging::PixelFormat::Format24bppRgb,(System::IntPtr) PCS_s.data);
					pB_ASVD->Refresh();
					
					pB_hist_src->Image  = gcnew    
					System::Drawing::Bitmap(hist.size().width,hist.size().height,hist.step,
					System::Drawing::Imaging::PixelFormat::Format24bppRgb,(System::IntPtr) hist.data);
					pB_hist_src->Refresh();

					pB_hist_dst->Image  = gcnew    
					System::Drawing::Bitmap(hist_dst.size().width,hist_dst.size().height,hist_dst.step,
					System::Drawing::Imaging::PixelFormat::Format24bppRgb,(System::IntPtr) hist_dst.data);
					pB_hist_dst->Refresh();
					step++;
				}
		 }
private: System::Void label31_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void textBox8_TextChanged(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void textBox13_TextChanged(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void textBox7_TextChanged(System::Object^  sender, System::EventArgs^  e) {
		 }
};
}

