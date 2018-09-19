#pragma once

#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\wavelib\wavelet2d.h"

#include <msclr/marshal_cppstd.h>


using namespace System::Runtime::InteropServices;
using namespace cv;
using namespace std;
using namespace System::Data;
using namespace System::Drawing;
using namespace msclr::interop;

//=======================================
// Tinh thoi gian
#define TIMER_INIT LARGE_INTEGER frequency;LARGE_INTEGER t1,t2;double elapsedTime;QueryPerformanceFrequency(&frequency);

// Use to start the performance timer /
#define TIMER_START QueryPerformanceCounter(&t1);

//** Use to stop the performance timer and output the result to the standard stream. Less verbose than \c TIMER_STOP_VERBOSE /
#define TIMER_STOP QueryPerformanceCounter(&t2);elapsedTime=(float)(t2.QuadPart-t1.QuadPart)/frequency.QuadPart;//std::wcout<<elapsedTime<<L" sec"<<endl;
//Ket thuc tinh thoi gian
//////////////////////////////////////////////////

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
//=======================================
static void* min_array(vector<vector<double> > &arr, double &min)
{
    min = 999999;
    for (unsigned int i =0; i < arr.size(); i++) 
	{
        for (unsigned int j =0; j < arr[0].size(); j++) 
		{
            if (min >= arr[i][j])
			{
                min = arr[i][j];
            }
        }
    }
    return 0;
}
//===========================================================

static double max3(double a, double b, double c)
{
	double max = a;
	max = max >= b ? max:b;
	max = max >= c ? max:c;
	return max;
}
//============================
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
//===============================
static Mat drawhistogram_gray(Mat img)
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

	Mat b_hist;

	/// Compute the histograms:
	calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
	//calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
	//calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );

	// Draw the histograms for B, G and R
	int hist_w = 1024; int hist_h = 400;
	int bin_w = cvRound( (double) hist_w/histSize );

	Mat histImage( hist_h, hist_w, CV_8UC3, Scalar(255,255,255) );//white, (0,0,0) is black
				
	/// Normalize the result to [ 0, histImage.rows ]
	normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
	//normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
	//normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

	// Draw for each channel
	for( int i = 1; i < histSize; i++ )
	{
		line( histImage, cv::Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
						 cv::Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
                         Scalar( 255, 0, 0), 2, 8, 0  );//blue
		//line( histImage, cv::Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
		//				 cv::Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
         //                Scalar( 0, 255, 0), 2, 8, 0  );//green
		//line( histImage, cv::Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
         //                cv::Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
         //                Scalar( 0, 0, 255), 2, 8, 0  );//red
	}
	return histImage;		
}

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

namespace ASVDW {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for frm_ASVDW
	/// </summary>
	public ref class frm_ASVDW : public System::Windows::Forms::Form
	{
	public:
		frm_ASVDW(void)
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
		~frm_ASVDW()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Panel^  panel1;
	protected: 

	private: System::Windows::Forms::Button^  btn_ASVDW;

	private: System::Windows::Forms::TextBox^  txt_savefolder;
	private: System::Windows::Forms::ComboBox^  cB_wavname;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::NumericUpDown^  nUD_level;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog;
	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::PictureBox^  pB_ori;

	private: System::Windows::Forms::PictureBox^  pB_wav;
	private: System::Windows::Forms::PictureBox^  pB_hist_src;
	private: System::Windows::Forms::PictureBox^  pB_hist_dst;
	private: System::Windows::Forms::PictureBox^  pB_dst_ASVDW;

	private: System::Windows::Forms::PictureBox^  pB_wav_ASVD;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  xi_LL_B;
	private: System::Windows::Forms::TextBox^  txt_xi_LL_B;
	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::TextBox^  txt_xi_LL_G;
	private: System::Windows::Forms::Label^  label11;
	private: System::Windows::Forms::TextBox^  txt_xi_LL_R;
	private: System::Windows::Forms::Label^  label12;
	private: System::Windows::Forms::TextBox^  txt_xi_LH_B;
	private: System::Windows::Forms::Label^  label13;
	private: System::Windows::Forms::TextBox^  txt_xi_LH_G;
	private: System::Windows::Forms::Label^  label14;
	private: System::Windows::Forms::TextBox^  txt_xi_LH_R;
	private: System::Windows::Forms::Label^  label15;
	private: System::Windows::Forms::TextBox^  txt_xi_HL_B;
	private: System::Windows::Forms::Label^  label16;
	private: System::Windows::Forms::TextBox^  txt_xi_HL_G;
	private: System::Windows::Forms::Label^  label17;
	private: System::Windows::Forms::TextBox^  txt_xi_HL_R;
	private: System::Windows::Forms::Label^  label18;
	private: System::Windows::Forms::TextBox^  txt_xi_HH_B;
	private: System::Windows::Forms::Label^  label19;
	private: System::Windows::Forms::TextBox^  txt_xi_HH_G;

	private: System::Windows::Forms::Label^  label20;
	private: System::Windows::Forms::TextBox^  txt_xi_HH_R;
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::Label^  label21;
	private: System::Windows::Forms::TextBox^  txt_pLP_img;

	private: System::Windows::Forms::Label^  label22;
	private: System::Windows::Forms::Label^  label23;
	private: System::Windows::Forms::Label^  label24;
	private: System::Windows::Forms::TextBox^  txt_pNP_img;

	private: System::Windows::Forms::Label^  label25;
	private: System::Windows::Forms::Label^  label26;
	private: System::Windows::Forms::TextBox^  txt_pHP_img;

	private: System::Windows::Forms::Label^  label27;
	private: System::Windows::Forms::TextBox^  txt_meanGa;
	private: System::Windows::Forms::Panel^  panel2;
	private: System::Windows::Forms::Panel^  panel3;
	private: System::Windows::Forms::ComboBox^  cB_dataname;
	private: System::Windows::Forms::Label^  label28;





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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(frm_ASVDW::typeid));
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->txt_savefolder = (gcnew System::Windows::Forms::TextBox());
			this->cB_dataname = (gcnew System::Windows::Forms::ComboBox());
			this->cB_wavname = (gcnew System::Windows::Forms::ComboBox());
			this->btn_ASVDW = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->nUD_level = (gcnew System::Windows::Forms::NumericUpDown());
			this->label28 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->openFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->pB_ori = (gcnew System::Windows::Forms::PictureBox());
			this->pB_wav = (gcnew System::Windows::Forms::PictureBox());
			this->pB_hist_src = (gcnew System::Windows::Forms::PictureBox());
			this->pB_hist_dst = (gcnew System::Windows::Forms::PictureBox());
			this->pB_dst_ASVDW = (gcnew System::Windows::Forms::PictureBox());
			this->pB_wav_ASVD = (gcnew System::Windows::Forms::PictureBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->xi_LL_B = (gcnew System::Windows::Forms::Label());
			this->txt_xi_LL_B = (gcnew System::Windows::Forms::TextBox());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->txt_xi_LL_G = (gcnew System::Windows::Forms::TextBox());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->txt_xi_LL_R = (gcnew System::Windows::Forms::TextBox());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->txt_xi_LH_B = (gcnew System::Windows::Forms::TextBox());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->txt_xi_LH_G = (gcnew System::Windows::Forms::TextBox());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->txt_xi_LH_R = (gcnew System::Windows::Forms::TextBox());
			this->label15 = (gcnew System::Windows::Forms::Label());
			this->txt_xi_HL_B = (gcnew System::Windows::Forms::TextBox());
			this->label16 = (gcnew System::Windows::Forms::Label());
			this->txt_xi_HL_G = (gcnew System::Windows::Forms::TextBox());
			this->label17 = (gcnew System::Windows::Forms::Label());
			this->txt_xi_HL_R = (gcnew System::Windows::Forms::TextBox());
			this->label18 = (gcnew System::Windows::Forms::Label());
			this->txt_xi_HH_B = (gcnew System::Windows::Forms::TextBox());
			this->label19 = (gcnew System::Windows::Forms::Label());
			this->txt_xi_HH_G = (gcnew System::Windows::Forms::TextBox());
			this->label20 = (gcnew System::Windows::Forms::Label());
			this->txt_xi_HH_R = (gcnew System::Windows::Forms::TextBox());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->label21 = (gcnew System::Windows::Forms::Label());
			this->txt_pLP_img = (gcnew System::Windows::Forms::TextBox());
			this->label22 = (gcnew System::Windows::Forms::Label());
			this->label23 = (gcnew System::Windows::Forms::Label());
			this->label24 = (gcnew System::Windows::Forms::Label());
			this->txt_pNP_img = (gcnew System::Windows::Forms::TextBox());
			this->label25 = (gcnew System::Windows::Forms::Label());
			this->label26 = (gcnew System::Windows::Forms::Label());
			this->txt_pHP_img = (gcnew System::Windows::Forms::TextBox());
			this->label27 = (gcnew System::Windows::Forms::Label());
			this->txt_meanGa = (gcnew System::Windows::Forms::TextBox());
			this->panel2 = (gcnew System::Windows::Forms::Panel());
			this->panel3 = (gcnew System::Windows::Forms::Panel());
			this->panel1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUD_level))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_ori))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_wav))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_hist_src))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_hist_dst))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_dst_ASVDW))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_wav_ASVD))->BeginInit();
			this->groupBox1->SuspendLayout();
			this->panel2->SuspendLayout();
			this->SuspendLayout();
			// 
			// panel1
			// 
			this->panel1->BackColor = System::Drawing::SystemColors::MenuBar;
			this->panel1->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->panel1->Controls->Add(this->txt_savefolder);
			this->panel1->Controls->Add(this->cB_dataname);
			this->panel1->Controls->Add(this->cB_wavname);
			this->panel1->Controls->Add(this->btn_ASVDW);
			this->panel1->Controls->Add(this->label1);
			this->panel1->Controls->Add(this->nUD_level);
			this->panel1->Controls->Add(this->label28);
			this->panel1->Controls->Add(this->label3);
			this->panel1->Controls->Add(this->label2);
			this->panel1->Location = System::Drawing::Point(34, 58);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(318, 200);
			this->panel1->TabIndex = 35;
			// 
			// txt_savefolder
			// 
			this->txt_savefolder->Location = System::Drawing::Point(107, 116);
			this->txt_savefolder->Name = L"txt_savefolder";
			this->txt_savefolder->Size = System::Drawing::Size(195, 20);
			this->txt_savefolder->TabIndex = 8;
			this->txt_savefolder->TextChanged += gcnew System::EventHandler(this, &frm_ASVDW::txt_savefolder_TextChanged);
			// 
			// cB_dataname
			// 
			this->cB_dataname->FormattingEnabled = true;
			this->cB_dataname->Items->AddRange(gcnew cli::array< System::Object^  >(4) {L"CMU-PIE", L"FERET", L"PIE", L"FEI"});
			this->cB_dataname->Location = System::Drawing::Point(111, 75);
			this->cB_dataname->Name = L"cB_dataname";
			this->cB_dataname->Size = System::Drawing::Size(144, 21);
			this->cB_dataname->TabIndex = 4;
			this->cB_dataname->SelectedIndexChanged += gcnew System::EventHandler(this, &frm_ASVDW::cB_dataname_SelectedIndexChanged);
			// 
			// cB_wavname
			// 
			this->cB_wavname->FormattingEnabled = true;
			this->cB_wavname->Items->AddRange(gcnew cli::array< System::Object^  >(45) {L"db1", L"db2", L"db3", L"db4", L"db5", L"db6", 
				L"db7", L"db8", L"db9", L"db10", L"db11", L"db12", L"db13", L"db14", L"db15", L"bior1.1", L"bior1.3", L"bior1.5", L"bior2.2", 
				L"bior2.4", L"bior2.6", L"bior2.8", L"bior3.1", L"bior3.3", L"bior3.5", L"bior3.7", L"bior3.9", L"bior4.4", L"bior5.5", L"bior6.8", 
				L"coif1", L"coif2", L"coif3", L"coif4", L"coif5", L"sym1", L"sym2", L"sym3", L"sym4", L"sym5", L"sym6", L"sym7", L"sym8", L"sym9", 
				L"sym10"});
			this->cB_wavname->Location = System::Drawing::Point(111, 41);
			this->cB_wavname->Name = L"cB_wavname";
			this->cB_wavname->Size = System::Drawing::Size(104, 21);
			this->cB_wavname->TabIndex = 4;
			this->cB_wavname->Text = L"db1";
			// 
			// btn_ASVDW
			// 
			this->btn_ASVDW->Location = System::Drawing::Point(16, 147);
			this->btn_ASVDW->Name = L"btn_ASVDW";
			this->btn_ASVDW->Size = System::Drawing::Size(286, 42);
			this->btn_ASVDW->TabIndex = 11;
			this->btn_ASVDW->Text = L"ASVDW";
			this->btn_ASVDW->UseVisualStyleBackColor = true;
			this->btn_ASVDW->Click += gcnew System::EventHandler(this, &frm_ASVDW::btn_ASVDW_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(13, 41);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(76, 13);
			this->label1->TabIndex = 5;
			this->label1->Text = L"Wavelet name";
			// 
			// nUD_level
			// 
			this->nUD_level->Location = System::Drawing::Point(111, 9);
			this->nUD_level->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->nUD_level->Name = L"nUD_level";
			this->nUD_level->Size = System::Drawing::Size(46, 20);
			this->nUD_level->TabIndex = 6;
			this->nUD_level->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			// 
			// label28
			// 
			this->label28->AutoSize = true;
			this->label28->Location = System::Drawing::Point(13, 77);
			this->label28->Name = L"label28";
			this->label28->Size = System::Drawing::Size(58, 13);
			this->label28->TabIndex = 5;
			this->label28->Text = L"Databases";
			this->label28->Click += gcnew System::EventHandler(this, &frm_ASVDW::label28_Click);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(9, 119);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(90, 13);
			this->label3->TabIndex = 5;
			this->label3->Text = L"Save folder name";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(17, 11);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(33, 13);
			this->label2->TabIndex = 5;
			this->label2->Text = L"Level";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(656, 28);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(47, 13);
			this->label5->TabIndex = 27;
			this->label5->Text = L"Wavelet";
			// 
			// openFileDialog
			// 
			this->openFileDialog->FileName = L"openFileDialog1";
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(864, 28);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(93, 13);
			this->label9->TabIndex = 30;
			this->label9->Text = L"Wavelet_ASVDW";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(1093, 28);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(47, 13);
			this->label6->TabIndex = 32;
			this->label6->Text = L"ASVDW";
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(963, 262);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(97, 13);
			this->label8->TabIndex = 29;
			this->label8->Text = L"ASVDW Histogram";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(533, 262);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(92, 13);
			this->label7->TabIndex = 28;
			this->label7->Text = L"Original Histogram";
			// 
			// pB_ori
			// 
			this->pB_ori->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->pB_ori->Location = System::Drawing::Point(370, 58);
			this->pB_ori->Name = L"pB_ori";
			this->pB_ori->Size = System::Drawing::Size(180, 200);
			this->pB_ori->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pB_ori->TabIndex = 25;
			this->pB_ori->TabStop = false;
			// 
			// pB_wav
			// 
			this->pB_wav->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->pB_wav->Location = System::Drawing::Point(590, 58);
			this->pB_wav->Name = L"pB_wav";
			this->pB_wav->Size = System::Drawing::Size(180, 200);
			this->pB_wav->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pB_wav->TabIndex = 24;
			this->pB_wav->TabStop = false;
			// 
			// pB_hist_src
			// 
			this->pB_hist_src->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->pB_hist_src->Location = System::Drawing::Point(370, 295);
			this->pB_hist_src->Name = L"pB_hist_src";
			this->pB_hist_src->Size = System::Drawing::Size(400, 175);
			this->pB_hist_src->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pB_hist_src->TabIndex = 33;
			this->pB_hist_src->TabStop = false;
			// 
			// pB_hist_dst
			// 
			this->pB_hist_dst->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->pB_hist_dst->Location = System::Drawing::Point(800, 295);
			this->pB_hist_dst->Name = L"pB_hist_dst";
			this->pB_hist_dst->Size = System::Drawing::Size(400, 175);
			this->pB_hist_dst->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pB_hist_dst->TabIndex = 34;
			this->pB_hist_dst->TabStop = false;
			// 
			// pB_dst_ASVDW
			// 
			this->pB_dst_ASVDW->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->pB_dst_ASVDW->Location = System::Drawing::Point(1020, 58);
			this->pB_dst_ASVDW->Name = L"pB_dst_ASVDW";
			this->pB_dst_ASVDW->Size = System::Drawing::Size(180, 200);
			this->pB_dst_ASVDW->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pB_dst_ASVDW->TabIndex = 26;
			this->pB_dst_ASVDW->TabStop = false;
			// 
			// pB_wav_ASVD
			// 
			this->pB_wav_ASVD->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->pB_wav_ASVD->Location = System::Drawing::Point(809, 58);
			this->pB_wav_ASVD->Name = L"pB_wav_ASVD";
			this->pB_wav_ASVD->Size = System::Drawing::Size(180, 200);
			this->pB_wav_ASVD->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pB_wav_ASVD->TabIndex = 23;
			this->pB_wav_ASVD->TabStop = false;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(435, 28);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(42, 13);
			this->label4->TabIndex = 31;
			this->label4->Text = L"Original";
			// 
			// xi_LL_B
			// 
			this->xi_LL_B->AutoSize = true;
			this->xi_LL_B->Location = System::Drawing::Point(21, 36);
			this->xi_LL_B->Name = L"xi_LL_B";
			this->xi_LL_B->Size = System::Drawing::Size(48, 13);
			this->xi_LL_B->TabIndex = 36;
			this->xi_LL_B->Text = L"xi_LL_B:";
			this->xi_LL_B->Click += gcnew System::EventHandler(this, &frm_ASVDW::xi_LL_B_Click);
			// 
			// txt_xi_LL_B
			// 
			this->txt_xi_LL_B->Location = System::Drawing::Point(75, 33);
			this->txt_xi_LL_B->Name = L"txt_xi_LL_B";
			this->txt_xi_LL_B->Size = System::Drawing::Size(98, 20);
			this->txt_xi_LL_B->TabIndex = 37;
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(21, 62);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(49, 13);
			this->label10->TabIndex = 36;
			this->label10->Text = L"xi_LL_G:";
			// 
			// txt_xi_LL_G
			// 
			this->txt_xi_LL_G->Location = System::Drawing::Point(75, 59);
			this->txt_xi_LL_G->Name = L"txt_xi_LL_G";
			this->txt_xi_LL_G->Size = System::Drawing::Size(98, 20);
			this->txt_xi_LL_G->TabIndex = 37;
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Location = System::Drawing::Point(21, 89);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(49, 13);
			this->label11->TabIndex = 36;
			this->label11->Text = L"xi_LL_R:";
			// 
			// txt_xi_LL_R
			// 
			this->txt_xi_LL_R->Location = System::Drawing::Point(75, 86);
			this->txt_xi_LL_R->Name = L"txt_xi_LL_R";
			this->txt_xi_LL_R->Size = System::Drawing::Size(98, 20);
			this->txt_xi_LL_R->TabIndex = 37;
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Location = System::Drawing::Point(229, 36);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(50, 13);
			this->label12->TabIndex = 36;
			this->label12->Text = L"xi_LH_B:";
			this->label12->Click += gcnew System::EventHandler(this, &frm_ASVDW::xi_LL_B_Click);
			// 
			// txt_xi_LH_B
			// 
			this->txt_xi_LH_B->Location = System::Drawing::Point(283, 33);
			this->txt_xi_LH_B->Name = L"txt_xi_LH_B";
			this->txt_xi_LH_B->Size = System::Drawing::Size(98, 20);
			this->txt_xi_LH_B->TabIndex = 37;
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Location = System::Drawing::Point(229, 62);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(51, 13);
			this->label13->TabIndex = 36;
			this->label13->Text = L"xi_LH_G:";
			this->label13->Click += gcnew System::EventHandler(this, &frm_ASVDW::xi_LL_B_Click);
			// 
			// txt_xi_LH_G
			// 
			this->txt_xi_LH_G->Location = System::Drawing::Point(283, 59);
			this->txt_xi_LH_G->Name = L"txt_xi_LH_G";
			this->txt_xi_LH_G->Size = System::Drawing::Size(98, 20);
			this->txt_xi_LH_G->TabIndex = 37;
			// 
			// label14
			// 
			this->label14->AutoSize = true;
			this->label14->Location = System::Drawing::Point(229, 89);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(51, 13);
			this->label14->TabIndex = 36;
			this->label14->Text = L"xi_LH_R:";
			this->label14->Click += gcnew System::EventHandler(this, &frm_ASVDW::xi_LL_B_Click);
			// 
			// txt_xi_LH_R
			// 
			this->txt_xi_LH_R->Location = System::Drawing::Point(283, 86);
			this->txt_xi_LH_R->Name = L"txt_xi_LH_R";
			this->txt_xi_LH_R->Size = System::Drawing::Size(98, 20);
			this->txt_xi_LH_R->TabIndex = 37;
			// 
			// label15
			// 
			this->label15->AutoSize = true;
			this->label15->Location = System::Drawing::Point(445, 36);
			this->label15->Name = L"label15";
			this->label15->Size = System::Drawing::Size(50, 13);
			this->label15->TabIndex = 36;
			this->label15->Text = L"xi_HL_B:";
			this->label15->Click += gcnew System::EventHandler(this, &frm_ASVDW::xi_LL_B_Click);
			// 
			// txt_xi_HL_B
			// 
			this->txt_xi_HL_B->Location = System::Drawing::Point(499, 33);
			this->txt_xi_HL_B->Name = L"txt_xi_HL_B";
			this->txt_xi_HL_B->Size = System::Drawing::Size(98, 20);
			this->txt_xi_HL_B->TabIndex = 37;
			// 
			// label16
			// 
			this->label16->AutoSize = true;
			this->label16->Location = System::Drawing::Point(445, 62);
			this->label16->Name = L"label16";
			this->label16->Size = System::Drawing::Size(51, 13);
			this->label16->TabIndex = 36;
			this->label16->Text = L"xi_HL_G:";
			this->label16->Click += gcnew System::EventHandler(this, &frm_ASVDW::xi_LL_B_Click);
			// 
			// txt_xi_HL_G
			// 
			this->txt_xi_HL_G->Location = System::Drawing::Point(499, 59);
			this->txt_xi_HL_G->Name = L"txt_xi_HL_G";
			this->txt_xi_HL_G->Size = System::Drawing::Size(98, 20);
			this->txt_xi_HL_G->TabIndex = 37;
			// 
			// label17
			// 
			this->label17->AutoSize = true;
			this->label17->Location = System::Drawing::Point(445, 85);
			this->label17->Name = L"label17";
			this->label17->Size = System::Drawing::Size(51, 13);
			this->label17->TabIndex = 36;
			this->label17->Text = L"xi_HL_R:";
			this->label17->Click += gcnew System::EventHandler(this, &frm_ASVDW::xi_LL_B_Click);
			// 
			// txt_xi_HL_R
			// 
			this->txt_xi_HL_R->Location = System::Drawing::Point(499, 82);
			this->txt_xi_HL_R->Name = L"txt_xi_HL_R";
			this->txt_xi_HL_R->Size = System::Drawing::Size(98, 20);
			this->txt_xi_HL_R->TabIndex = 37;
			// 
			// label18
			// 
			this->label18->AutoSize = true;
			this->label18->Location = System::Drawing::Point(658, 36);
			this->label18->Name = L"label18";
			this->label18->Size = System::Drawing::Size(52, 13);
			this->label18->TabIndex = 36;
			this->label18->Text = L"xi_HH_B:";
			this->label18->Click += gcnew System::EventHandler(this, &frm_ASVDW::xi_LL_B_Click);
			// 
			// txt_xi_HH_B
			// 
			this->txt_xi_HH_B->Location = System::Drawing::Point(712, 33);
			this->txt_xi_HH_B->Name = L"txt_xi_HH_B";
			this->txt_xi_HH_B->Size = System::Drawing::Size(98, 20);
			this->txt_xi_HH_B->TabIndex = 37;
			// 
			// label19
			// 
			this->label19->AutoSize = true;
			this->label19->Location = System::Drawing::Point(658, 58);
			this->label19->Name = L"label19";
			this->label19->Size = System::Drawing::Size(53, 13);
			this->label19->TabIndex = 36;
			this->label19->Text = L"xi_HH_G:";
			this->label19->Click += gcnew System::EventHandler(this, &frm_ASVDW::xi_LL_B_Click);
			// 
			// txt_xi_HH_G
			// 
			this->txt_xi_HH_G->Location = System::Drawing::Point(712, 55);
			this->txt_xi_HH_G->Name = L"txt_xi_HH_G";
			this->txt_xi_HH_G->Size = System::Drawing::Size(98, 20);
			this->txt_xi_HH_G->TabIndex = 37;
			// 
			// label20
			// 
			this->label20->AutoSize = true;
			this->label20->Location = System::Drawing::Point(658, 81);
			this->label20->Name = L"label20";
			this->label20->Size = System::Drawing::Size(53, 13);
			this->label20->TabIndex = 36;
			this->label20->Text = L"xi_HH_R:";
			this->label20->Click += gcnew System::EventHandler(this, &frm_ASVDW::xi_LL_B_Click);
			// 
			// txt_xi_HH_R
			// 
			this->txt_xi_HH_R->Location = System::Drawing::Point(712, 80);
			this->txt_xi_HH_R->Name = L"txt_xi_HH_R";
			this->txt_xi_HH_R->Size = System::Drawing::Size(98, 20);
			this->txt_xi_HH_R->TabIndex = 37;
			this->txt_xi_HH_R->TextChanged += gcnew System::EventHandler(this, &frm_ASVDW::textBox2_TextChanged);
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->txt_xi_LL_R);
			this->groupBox1->Controls->Add(this->label11);
			this->groupBox1->Controls->Add(this->txt_xi_LL_G);
			this->groupBox1->Controls->Add(this->label10);
			this->groupBox1->Controls->Add(this->txt_xi_LH_R);
			this->groupBox1->Controls->Add(this->label14);
			this->groupBox1->Controls->Add(this->txt_xi_LH_G);
			this->groupBox1->Controls->Add(this->label13);
			this->groupBox1->Controls->Add(this->txt_xi_HL_R);
			this->groupBox1->Controls->Add(this->label17);
			this->groupBox1->Controls->Add(this->txt_xi_HL_G);
			this->groupBox1->Controls->Add(this->label16);
			this->groupBox1->Controls->Add(this->txt_xi_HH_R);
			this->groupBox1->Controls->Add(this->txt_xi_HH_G);
			this->groupBox1->Controls->Add(this->label20);
			this->groupBox1->Controls->Add(this->txt_xi_HH_B);
			this->groupBox1->Controls->Add(this->label19);
			this->groupBox1->Controls->Add(this->txt_xi_HL_B);
			this->groupBox1->Controls->Add(this->label18);
			this->groupBox1->Controls->Add(this->label15);
			this->groupBox1->Controls->Add(this->txt_xi_LH_B);
			this->groupBox1->Controls->Add(this->label12);
			this->groupBox1->Controls->Add(this->txt_xi_LL_B);
			this->groupBox1->Controls->Add(this->xi_LL_B);
			this->groupBox1->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->groupBox1->Location = System::Drawing::Point(372, 475);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(827, 128);
			this->groupBox1->TabIndex = 38;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Result parameters";
			// 
			// label21
			// 
			this->label21->AutoSize = true;
			this->label21->Location = System::Drawing::Point(34, 23);
			this->label21->Name = L"label21";
			this->label21->Size = System::Drawing::Size(48, 13);
			this->label21->TabIndex = 36;
			this->label21->Text = L"pLP_img";
			this->label21->Click += gcnew System::EventHandler(this, &frm_ASVDW::xi_LL_B_Click);
			// 
			// txt_pLP_img
			// 
			this->txt_pLP_img->Location = System::Drawing::Point(125, 20);
			this->txt_pLP_img->Name = L"txt_pLP_img";
			this->txt_pLP_img->Size = System::Drawing::Size(110, 20);
			this->txt_pLP_img->TabIndex = 37;
			this->txt_pLP_img->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			// 
			// label22
			// 
			this->label22->AutoSize = true;
			this->label22->Location = System::Drawing::Point(241, 23);
			this->label22->Name = L"label22";
			this->label22->Size = System::Drawing::Size(15, 13);
			this->label22->TabIndex = 36;
			this->label22->Text = L"%";
			this->label22->Click += gcnew System::EventHandler(this, &frm_ASVDW::xi_LL_B_Click);
			// 
			// label23
			// 
			this->label23->AutoSize = true;
			this->label23->Location = System::Drawing::Point(34, 49);
			this->label23->Name = L"label23";
			this->label23->Size = System::Drawing::Size(50, 13);
			this->label23->TabIndex = 36;
			this->label23->Text = L"pNP_img";
			this->label23->Click += gcnew System::EventHandler(this, &frm_ASVDW::xi_LL_B_Click);
			// 
			// label24
			// 
			this->label24->AutoSize = true;
			this->label24->Location = System::Drawing::Point(241, 49);
			this->label24->Name = L"label24";
			this->label24->Size = System::Drawing::Size(15, 13);
			this->label24->TabIndex = 36;
			this->label24->Text = L"%";
			this->label24->Click += gcnew System::EventHandler(this, &frm_ASVDW::xi_LL_B_Click);
			// 
			// txt_pNP_img
			// 
			this->txt_pNP_img->Location = System::Drawing::Point(125, 46);
			this->txt_pNP_img->Name = L"txt_pNP_img";
			this->txt_pNP_img->Size = System::Drawing::Size(110, 20);
			this->txt_pNP_img->TabIndex = 37;
			this->txt_pNP_img->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			// 
			// label25
			// 
			this->label25->AutoSize = true;
			this->label25->Location = System::Drawing::Point(34, 75);
			this->label25->Name = L"label25";
			this->label25->Size = System::Drawing::Size(50, 13);
			this->label25->TabIndex = 36;
			this->label25->Text = L"pHP_img";
			this->label25->Click += gcnew System::EventHandler(this, &frm_ASVDW::xi_LL_B_Click);
			// 
			// label26
			// 
			this->label26->AutoSize = true;
			this->label26->Location = System::Drawing::Point(241, 75);
			this->label26->Name = L"label26";
			this->label26->Size = System::Drawing::Size(15, 13);
			this->label26->TabIndex = 36;
			this->label26->Text = L"%";
			this->label26->Click += gcnew System::EventHandler(this, &frm_ASVDW::xi_LL_B_Click);
			// 
			// txt_pHP_img
			// 
			this->txt_pHP_img->Location = System::Drawing::Point(125, 72);
			this->txt_pHP_img->Name = L"txt_pHP_img";
			this->txt_pHP_img->Size = System::Drawing::Size(110, 20);
			this->txt_pHP_img->TabIndex = 37;
			this->txt_pHP_img->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			// 
			// label27
			// 
			this->label27->AutoSize = true;
			this->label27->Location = System::Drawing::Point(34, 101);
			this->label27->Name = L"label27";
			this->label27->Size = System::Drawing::Size(47, 13);
			this->label27->TabIndex = 36;
			this->label27->Text = L"meanGa";
			this->label27->Click += gcnew System::EventHandler(this, &frm_ASVDW::xi_LL_B_Click);
			// 
			// txt_meanGa
			// 
			this->txt_meanGa->Location = System::Drawing::Point(125, 98);
			this->txt_meanGa->Name = L"txt_meanGa";
			this->txt_meanGa->Size = System::Drawing::Size(110, 20);
			this->txt_meanGa->TabIndex = 37;
			this->txt_meanGa->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			// 
			// panel2
			// 
			this->panel2->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->panel2->Controls->Add(this->txt_meanGa);
			this->panel2->Controls->Add(this->txt_pHP_img);
			this->panel2->Controls->Add(this->label26);
			this->panel2->Controls->Add(this->txt_pNP_img);
			this->panel2->Controls->Add(this->label27);
			this->panel2->Controls->Add(this->label24);
			this->panel2->Controls->Add(this->label25);
			this->panel2->Controls->Add(this->txt_pLP_img);
			this->panel2->Controls->Add(this->label23);
			this->panel2->Controls->Add(this->label22);
			this->panel2->Controls->Add(this->label21);
			this->panel2->Location = System::Drawing::Point(34, 457);
			this->panel2->Name = L"panel2";
			this->panel2->Size = System::Drawing::Size(318, 146);
			this->panel2->TabIndex = 39;
			// 
			// panel3
			// 
			this->panel3->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"panel3.BackgroundImage")));
			this->panel3->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->panel3->Location = System::Drawing::Point(34, 295);
			this->panel3->Name = L"panel3";
			this->panel3->Size = System::Drawing::Size(311, 121);
			this->panel3->TabIndex = 40;
			// 
			// frm_ASVDW
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1224, 683);
			this->Controls->Add(this->panel3);
			this->Controls->Add(this->panel2);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->panel1);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->label9);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->label8);
			this->Controls->Add(this->label7);
			this->Controls->Add(this->pB_ori);
			this->Controls->Add(this->pB_wav);
			this->Controls->Add(this->pB_hist_src);
			this->Controls->Add(this->pB_hist_dst);
			this->Controls->Add(this->pB_dst_ASVDW);
			this->Controls->Add(this->pB_wav_ASVD);
			this->Controls->Add(this->label4);
			this->Name = L"frm_ASVDW";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"frm_ASVDW";
			this->panel1->ResumeLayout(false);
			this->panel1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nUD_level))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_ori))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_wav))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_hist_src))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_hist_dst))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_dst_ASVDW))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_wav_ASVD))->EndInit();
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->panel2->ResumeLayout(false);
			this->panel2->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void btn_ASVDW_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				System::String^ pathfolder; 
				//txt_savefolder->Text= cB_dataname->Text;
				if (txt_savefolder->Text== "")
				{
					MessageBox::Show("Please put the Folder to save ASVDW images","Color Face Image Enhancement Using ASVDW", MessageBoxButtons::OK,MessageBoxIcon::Asterisk);
					return;
				}
				else
				{
					pathfolder ="..\\" + txt_savefolder->Text; 
					marshal_context context;
					LPCTSTR cstr = context.marshal_as<const TCHAR*>(pathfolder);
					CreateDirectory (cstr, NULL);
				}

				// Open Image file
				openFileDialog->Filter = "Image Files|*.bmp; *.jpg; *.png|All Files (*.*)|*.*||"; 
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


				//Wavelet name.
				msclr::interop::marshal_context context;
				string nm = context.marshal_as<std::string>(cB_wavname->Text);
				// Wavelet level value
				int J = (int)nUD_level->Value;

				System::String^ OpenFileName;
				 			
				int step = 0;

				while (step <number_of_files)
				{		 
				
				
					OpenFileName = filename_src[step];
					//lstBox->Items->Add(OpenFileName);
					OpenFileName = OpenFileName->Replace("\\","\\\\");
					char* imgname = (char*)Marshal::StringToHGlobalAnsi(OpenFileName).ToPointer();  // convert to kieu char lay ten filecho opencv
				
					Mat img = cv::imread(imgname);
				
				
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
				TIMER_INIT
				TIMER_START
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
					
					// End of computing wavelet subband image (for display)

					//ASVD
					output_ASVD_B = output_B;
					output_ASVD_G = output_G;
					output_ASVD_R = output_R;
				
					// convert from 1D vector to 2D matrix
					int rows_1 = length_B[0];
					int cols_1 = length_B[1];
					Mat LL_B = Mat(rows_1, cols_1, CV_64F);
					Mat HL_B = Mat(rows_1, cols_1, CV_64F);
					Mat LH_B = Mat(rows_1, cols_1, CV_64F);
					Mat HH_B = Mat(rows_1, cols_1, CV_64F);

					Mat LL_G = Mat(rows_1, cols_1, CV_64F);
					Mat HL_G = Mat(rows_1, cols_1, CV_64F);
					Mat LH_G = Mat(rows_1, cols_1, CV_64F);
					Mat HH_G = Mat(rows_1, cols_1, CV_64F);

					Mat LL_R = Mat(rows_1, cols_1, CV_64F);
					Mat HL_R = Mat(rows_1, cols_1, CV_64F);
					Mat LH_R = Mat(rows_1, cols_1, CV_64F);
					Mat HH_R = Mat(rows_1, cols_1, CV_64F);
				
					Mat LL_Ga = Mat(rows_1, cols_1, CV_64F);
					Mat HL_Ga = Mat(rows_1, cols_1, CV_64F);
					Mat LH_Ga = Mat(rows_1, cols_1, CV_64F);
					Mat HH_Ga = Mat(rows_1, cols_1, CV_64F);
					// convert to each subband

					for (int k = 0; k < rows_1*cols_1; k++)
					{
						int i = k/cols_1; // thuong->dong
						int j = k%cols_1; // du->cot
						LL_B.at<double>(cv::Point(j, i)) = output_ASVD_B[k];
						LL_G.at<double>(cv::Point(j, i)) = output_ASVD_G[k];
						LL_R.at<double>(cv::Point(j, i)) = output_ASVD_R[k];
						LL_Ga.at<double>(cv::Point(j, i)) = output_Ga[k];
					
					}

					// compute mean of positive value
					double meanLL_B = mean_pos(LL_B);
					double meanLL_G = mean_pos(LL_G);
					double meanLL_R = mean_pos(LL_R);
					double max_mean_LL = max3(meanLL_B,meanLL_G,meanLL_R);

					for (int k = rows_1*cols_1; k < 2*rows_1*cols_1; k++)
					{
						int k1 = k - rows_1*cols_1;
						int i = k1/cols_1; // thuong->dong
						int j = k1%cols_1; // du->cot
						HL_B.at<double>(cv::Point(j, i)) = output_ASVD_B[k];
						HL_G.at<double>(cv::Point(j, i)) = output_ASVD_G[k];
						HL_R.at<double>(cv::Point(j, i)) = output_ASVD_R[k];
						HL_Ga.at<double>(cv::Point(j, i)) = output_Ga[k];
					
					}

					double meanHL_B = mean_pos(HL_B);
					double meanHL_G = mean_pos(HL_G);
					double meanHL_R = mean_pos(HL_R);
					double max_mean_HL = max3(meanHL_B, meanHL_G, meanHL_R);

					// mean of LHs
				
				
					for (int k = 2*rows_1*cols_1; k < 3*rows_1*cols_1; k++)
					{
						int k1 = k - 2*rows_1*cols_1;
						int i = k1/cols_1; // thuong->dong
						int j = k1%cols_1; // du->cot
					
						LH_B.at<double>(cv::Point(j, i)) = output_ASVD_B[k];
						LH_G.at<double>(cv::Point(j, i)) = output_ASVD_G[k];
						LH_R.at<double>(cv::Point(j, i)) = output_ASVD_R[k];
						LH_Ga.at<double>(cv::Point(j, i)) = output_Ga[k];
					
					}

					double meanLH_B = mean_pos(LH_B);
					double meanLH_G = mean_pos(LH_G);
					double meanLH_R = mean_pos(LH_R);
					double max_mean_LH = max3(meanLH_B, meanLH_G, meanLH_R);

					//
					for (int k = 3*rows_1*cols_1; k < 4*rows_1*cols_1; k++)
					{
						int k1 = k - 3*rows_1*cols_1;
						int i = k1/cols_1; // thuong->dong
						int j = k1%cols_1; // du->cot
					
						HH_B.at<double>(cv::Point(j, i)) = output_ASVD_B[k];
						HH_G.at<double>(cv::Point(j, i)) = output_ASVD_G[k];
						HH_R.at<double>(cv::Point(j, i)) = output_ASVD_R[k];
						HH_Ga.at<double>(cv::Point(j, i)) = output_Ga[k];
					
					}

					double meanHH_B = mean_pos(HH_B);
					double meanHH_G = mean_pos(HH_G);
					double meanHH_R = mean_pos(HH_R);
					double max_mean_HH = max3(meanHH_B, meanHH_G, meanHH_R);

					// compute SVD of each subband of Gaussian matrix				
					Mat wLL_Ga, uLL_Ga, vtLL_Ga;
					SVD::compute(LL_Ga,wLL_Ga, uLL_Ga, vtLL_Ga, SVD::NO_UV);
					double max_wLL_Ga = wLL_Ga.at<double>(cv::Point(0, 0));

					Mat wHL_Ga, uHL_Ga, vtHL_Ga;
					SVD::compute(HL_Ga,wHL_Ga, uHL_Ga, vtHL_Ga, SVD::FULL_UV);
					double max_wHL_Ga = wHL_Ga.at<double>(cv::Point(0, 0));

					Mat wLH_Ga, uLH_Ga, vtLH_Ga;
					SVD::compute(LH_Ga,wLH_Ga, uLH_Ga, vtLH_Ga, SVD::FULL_UV);
					double max_wLH_Ga = wLH_Ga.at<double>(cv::Point(0, 0));

					Mat wHH_Ga, uHH_Ga, vtHH_Ga;
					SVD::compute(HH_Ga,wHH_Ga, uHH_Ga, vtHH_Ga, SVD::FULL_UV);
					double max_wHH_Ga = wHH_Ga.at<double>(cv::Point(0, 0));
				
								
					//LL_Blue
					SVD svd(LL_B, SVD::MODIFY_A);

					Mat wLL_B, uLL_B, vtLL_B;
					SVD::compute(LL_B,wLL_B, uLL_B, vtLL_B, SVD::FULL_UV);
					Mat wLL_G, uLL_G, vtLL_G;
					SVD::compute(LL_G,wLL_G, uLL_G, vtLL_G, SVD::FULL_UV);
					Mat wLL_R, uLL_R, vtLL_R;
					SVD::compute(LL_R,wLL_R, uLL_R, vtLL_R, SVD::FULL_UV);
				
					double max_wLL_B = wLL_B.at<double>(cv::Point(0,0));
					double max_wLL_G = wLL_G.at<double>(cv::Point(0,0));
					double max_wLL_R = wLL_R.at<double>(cv::Point(0,0));

					double xi_LL_B = (double) sqrtf((max_mean_LL/meanLL_B)*(max_wLL_Ga/max_wLL_B));
					double xi_LL_G = (double) sqrtf((max_mean_LL/meanLL_G)*(max_wLL_Ga/max_wLL_G));
					double xi_LL_R = (double) sqrtf((max_mean_LL/meanLL_R)*(max_wLL_Ga/max_wLL_R));

					for (int i = 0; i < svd.w.cols; i++)
					{
						for (int j = 0; j < svd.w.rows; j ++)
						{
						
							svd.w.at<double>(cv::Point(i, j))=xi_LL_B*wLL_B.at<double>(cv::Point(i, j));
							
						}
					}
					for (int i = 0; i < svd.u.cols; i++)
					{
						for (int j = 0; j < svd.u.rows; j ++)
						{
							svd.u.at<double>(cv::Point(i, j)) = uLL_B.at<double>(cv::Point(i, j));
						}
					}
					for (int i = 0; i < svd.vt.cols; i++)
					{
						for (int j = 0; j < svd.vt.rows; j ++)
						{
							svd.vt.at<double>(cv::Point(i, j)) = vtLL_B.at<double>(cv::Point(i, j));
						}
					}
				
					Mat LL1_ASVD_B = svd.u*Mat::diag(svd.w)*svd.vt;
				
					//===================================================================================
				
					for (int i = 0; i < svd.w.cols; i++)
					{
						for (int j = 0; j < svd.w.rows; j ++)
						{
						
							svd.w.at<double>(cv::Point(i, j))=xi_LL_G*wLL_G.at<double>(cv::Point(i, j));
						
						}
					}
				
					for (int i = 0; i < svd.u.cols; i++)
					{
						for (int j = 0; j < svd.u.rows; j ++)
						{
							svd.u.at<double>(cv::Point(i, j)) = uLL_G.at<double>(cv::Point(i, j));
						}
					}
					for (int i = 0; i < svd.vt.cols; i++)
					{
						for (int j = 0; j < svd.vt.rows; j ++)
						{
							svd.vt.at<double>(cv::Point(i, j)) = vtLL_G.at<double>(cv::Point(i, j));
						}
					}
				
					Mat LL1_ASVD_G = svd.u*Mat::diag(svd.w)*svd.vt;
				
					//===================================================================================
				
					for (int i = 0; i < svd.w.cols; i++)
					{
						for (int j = 0; j < svd.w.rows; j ++)
						{
							svd.w.at<double>(cv::Point(i, j))=xi_LL_R*wLL_R.at<double>(cv::Point(i, j));
										
						}
					}
				
					for (int i = 0; i < svd.u.cols; i++)
					{
						for (int j = 0; j < svd.u.rows; j ++)
						{
							svd.u.at<double>(cv::Point(i, j)) = uLL_R.at<double>(cv::Point(i, j));
						}
					}
					for (int i = 0; i < svd.vt.cols; i++)
					{
						for (int j = 0; j < svd.vt.rows; j ++)
						{
							svd.vt.at<double>(cv::Point(i, j)) = vtLL_R.at<double>(cv::Point(i, j));
						}
					}
				
					Mat LL1_ASVD_R = svd.u*Mat::diag(svd.w)*svd.vt;
					//===================================================================================
					// ASVD of HL1
				
					Mat wHL_B, uHL_B, vtHL_B;
					SVD::compute(HL_B,wHL_B, uHL_B, vtHL_B, SVD::FULL_UV);
					double max_wHL_B = wHL_B.at<double>(cv::Point(0,0));
				
					Mat wHL_G, uHL_G, vtHL_G;
					SVD::compute(HL_G,wHL_G, uHL_G, vtHL_G, SVD::FULL_UV);
					double max_wHL_G = wHL_G.at<double>(cv::Point(0,0));
				
					Mat wHL_R, uHL_R, vtHL_R;
					SVD::compute(HL_R,wHL_R, uHL_R, vtHL_R, SVD::FULL_UV);
					double max_wHL_R = wHL_R.at<double>(cv::Point(0,0));

					///////////////////////////////////////////
				
					Mat wLH_B, uLH_B, vtLH_B;
					SVD::compute(LH_B,wLH_B, uLH_B, vtLH_B, SVD::FULL_UV);
					double max_wLH_B = wLH_B.at<double>(cv::Point(0,0));
							
					Mat wLH_G, uLH_G, vtLH_G;
					SVD::compute(LH_G,wLH_G, uLH_G, vtLH_G, SVD::FULL_UV);
					double max_wLH_G = wLH_G.at<double>(cv::Point(0,0));
				
					Mat wLH_R, uLH_R, vtLH_R;
					SVD::compute(LH_R,wLH_R, uLH_R, vtLH_R, SVD::FULL_UV);
					double max_wLH_R = wLH_R.at<double>(cv::Point(0,0));

				
					////////////////////////////////////////////////////////////////
					Mat wHH_B, uHH_B, vtHH_B;
					SVD::compute(HH_B,wHH_B, uHH_B, vtHH_B, SVD::FULL_UV);
					double max_wHH_B = wHH_B.at<double>(cv::Point(0,0));
												
					Mat wHH_G, uHH_G, vtHH_G;
					SVD::compute(HH_G,wHH_G, uHH_G, vtHH_G, SVD::FULL_UV);
					double max_wHH_G = wHH_G.at<double>(cv::Point(0,0));
				
					Mat wHH_R, uHH_R, vtHH_R;
					SVD::compute(HH_R,wHH_R, uHH_R, vtHH_R, SVD::FULL_UV);
					double max_wHH_R = wHH_R.at<double>(cv::Point(0,0));

				
					//////////////////////////////////////////////////////////////////////////////////////////////////
				
					double xi_HL_B = (double) (1+ (max_mean_HL/meanHL_B)*(max_wHL_Ga/max_wHL_B));
					double xi_HL_G = (double) (1+ (max_mean_HL/meanHL_G)*(max_wHL_Ga/max_wHL_G));
					double xi_HL_R = (double) (1+ (max_mean_HL/meanHL_R)*(max_wHL_Ga/max_wHL_R));

					//double xi_HL_B = (double) pow(((meanLL_B/meanHL_B)*(max_wHL_Ga/max_wHL_B)),1.0/6.0);
					//double xi_HL_G = (double) pow(((meanLL_G/meanHL_G)*(max_wHL_Ga/max_wHL_G)),1.0/6.0);
					//double xi_HL_R = (double) pow(((meanLL_R/meanHL_R)*(max_wHL_Ga/max_wHL_R)),1.0/6.0);
					
					//=============
					double xi_LH_B = (double) (1+ (max_mean_LH/meanLH_B)*(max_wLH_Ga/max_wLH_B));
					double xi_LH_G = (double) (1+ (max_mean_LH/meanLH_G)*(max_wLH_Ga/max_wLH_G));
					double xi_LH_R = (double) (1+ (max_mean_LH/meanLH_R)*(max_wLH_Ga/max_wLH_R));
					//double xi_LH_B = (double) pow(((meanLL_B/meanLH_B)*(max_wLH_Ga/max_wLH_B)),1.0/6.0);
					//double xi_LH_G = (double) pow(((meanLL_G/meanLH_G)*(max_wLH_Ga/max_wLH_G)),1.0/6.0);
					//double xi_LH_R = (double) pow(((meanLL_R/meanLH_R)*(max_wLH_Ga/max_wLH_R)),1.0/6.0);
					
					//==============
					double xi_HH_B = (double) ((max_mean_HH/meanHH_B)*(max_wHH_Ga/max_wHH_B));
					double xi_HH_G = (double) ((max_mean_HH/meanHH_G)*(max_wHH_Ga/max_wHH_G));
					double xi_HH_R = (double) ((max_mean_HH/meanHH_R)*(max_wHH_Ga/max_wHH_R));

					/////////////////////////////////////////////////////////////////////////////
				
					for (int i = 0; i < svd.w.cols; i++)
					{
						for (int j = 0; j < svd.w.rows; j ++)
						{
						
							svd.w.at<double>(cv::Point(i, j))=xi_HL_B*wHL_B.at<double>(cv::Point(i, j));
							//svd.w.at<double>(cv::Point(i, j)) =xi_HL_B*wHL_Ga.at<double>(cv::Point(i, j));
							//svd.w.at<double>(cv::Point(i, j))=wHL_B.at<double>(cv::Point(i, j));
						}
					}
				
					for (int i = 0; i < svd.u.cols; i++)
					{
						for (int j = 0; j < svd.u.rows; j ++)
						{
							svd.u.at<double>(cv::Point(i, j)) = uHL_B.at<double>(cv::Point(i, j));
						}
					}
					for (int i = 0; i < svd.vt.cols; i++)
					{
						for (int j = 0; j < svd.vt.rows; j ++)
						{
							svd.vt.at<double>(cv::Point(i, j)) = vtHL_B.at<double>(cv::Point(i, j));
						}
					}
				
					Mat HL1_ASVD_B = svd.u*Mat::diag(svd.w)*svd.vt;
				
					//===================
				
				
					for (int i = 0; i < svd.w.cols; i++)
					{
						for (int j = 0; j < svd.w.rows; j ++)
						{
						
							svd.w.at<double>(cv::Point(i, j))=xi_HL_G*wHL_G.at<double>(cv::Point(i, j));
							//svd.w.at<double>(cv::Point(i, j))=xi_HL_G*wHL_Ga.at<double>(cv::Point(i, j));
							//svd.w.at<double>(cv::Point(i, j))=wHL_G.at<double>(cv::Point(i, j));
						}
					}
				
					for (int i = 0; i < svd.u.cols; i++)
					{
						for (int j = 0; j < svd.u.rows; j ++)
						{
							svd.u.at<double>(cv::Point(i, j)) = uHL_G.at<double>(cv::Point(i, j));
						}
					}
					for (int i = 0; i < svd.vt.cols; i++)
					{
						for (int j = 0; j < svd.vt.rows; j ++)
						{
							svd.vt.at<double>(cv::Point(i, j)) = vtHL_G.at<double>(cv::Point(i, j));
						}
					}

					Mat HL1_ASVD_G = svd.u*Mat::diag(svd.w)*svd.vt;
				
					//=====================================
		
					for (int i = 0; i < svd.w.cols; i++)
					{
						for (int j = 0; j < svd.w.rows; j ++)
						{
						
							svd.w.at<double>(cv::Point(i, j))=xi_HL_R*wHL_R.at<double>(cv::Point(i, j));
							//svd.w.at<double>(cv::Point(i, j))=xi_HL_R*wHL_Ga.at<double>(cv::Point(i, j));
							//svd.w.at<double>(cv::Point(i, j))=wHL_R.at<double>(cv::Point(i, j));
						}
					}
				
					for (int i = 0; i < svd.u.cols; i++)
					{
						for (int j = 0; j < svd.u.rows; j ++)
						{
							svd.u.at<double>(cv::Point(i, j)) = uHL_R.at<double>(cv::Point(i, j));
						}
					}
					for (int i = 0; i < svd.vt.cols; i++)
					{
						for (int j = 0; j < svd.vt.rows; j ++)
						{
							svd.vt.at<double>(cv::Point(i, j)) = vtHL_R.at<double>(cv::Point(i, j));
						}
					}
				
					Mat HL1_ASVD_R = svd.u*Mat::diag(svd.w)*svd.vt;
				
					//====================

				
					for (int i = 0; i < svd.w.cols; i++)
					{
						for (int j = 0; j < svd.w.rows; j ++)
						{
						
							svd.w.at<double>(cv::Point(i, j))=xi_LH_B*wLH_B.at<double>(cv::Point(i, j));
							//svd.w.at<double>(cv::Point(i, j))=xi_LH_B*wLH_Ga.at<double>(cv::Point(i, j));
							//svd.w.at<double>(cv::Point(i, j)) = wLH_B.at<double>(cv::Point(i, j));
						}
					}
				
					for (int i = 0; i < svd.u.cols; i++)
					{
						for (int j = 0; j < svd.u.rows; j ++)
						{
							svd.u.at<double>(cv::Point(i, j)) = uLH_B.at<double>(cv::Point(i, j));
						}
					}
				
					for (int i = 0; i < svd.vt.cols; i++)
					{
						for (int j = 0; j < svd.vt.rows; j ++)
						{
							svd.vt.at<double>(cv::Point(i, j)) = vtLH_B.at<double>(cv::Point(i, j));
						}
					}
				
					Mat LH1_ASVD_B = svd.u*Mat::diag(svd.w)*svd.vt;
				
					//================================
					for (int i = 0; i < svd.w.cols; i++)
					{
						for (int j = 0; j < svd.w.rows; j ++)
						{
						
							svd.w.at<double>(cv::Point(i, j))=xi_LH_G*wLH_G.at<double>(cv::Point(i, j));
							//svd.w.at<double>(cv::Point(i, j))=xi_LH_G*wLH_Ga.at<double>(cv::Point(i, j));
							//svd.w.at<double>(cv::Point(i, j)) = wLH_G.at<double>(cv::Point(i, j));
						}
					}
				
					for (int i = 0; i < svd.u.cols; i++)
					{
						for (int j = 0; j < svd.u.rows; j ++)
						{
							svd.u.at<double>(cv::Point(i, j)) = uLH_G.at<double>(cv::Point(i, j));
						}
					}
				
					for (int i = 0; i < svd.vt.cols; i++)
					{
						for (int j = 0; j < svd.vt.rows; j ++)
						{
							svd.vt.at<double>(cv::Point(i, j)) = vtLH_G.at<double>(cv::Point(i, j));
						}
					}
				
				
					Mat LH1_ASVD_G = svd.u*Mat::diag(svd.w)*svd.vt;
					//==================================

				
					for (int i = 0; i < svd.w.cols; i++)
					{
						for (int j = 0; j < svd.w.rows; j ++)
						{
						
							svd.w.at<double>(cv::Point(i, j))=xi_LH_R*wLH_R.at<double>(cv::Point(i, j));
							//svd.w.at<double>(cv::Point(i, j))=xi_LH_R*wLH_Ga.at<double>(cv::Point(i, j));
							//svd.w.at<double>(cv::Point(i, j)) = wLH_R.at<double>(cv::Point(i, j));
						}
					}
				
					for (int i = 0; i < svd.u.cols; i++)
					{
						for (int j = 0; j < svd.u.rows; j ++)
						{
							svd.u.at<double>(cv::Point(i, j)) = uLH_R.at<double>(cv::Point(i, j));
						}
					}
					for (int i = 0; i < svd.vt.cols; i++)
					{
						for (int j = 0; j < svd.vt.rows; j ++)
						{
							svd.vt.at<double>(cv::Point(i, j)) = vtLH_R.at<double>(cv::Point(i, j));
						}
					}
				
					Mat LH1_ASVD_R = svd.u*Mat::diag(svd.w)*svd.vt;
				
					//=======================

				
					for (int i = 0; i < svd.w.cols; i++)
					{
						for (int j = 0; j < svd.w.rows; j ++)
						{
							svd.w.at<double>(cv::Point(i, j))=xi_HH_B*wHH_B.at<double>(cv::Point(i, j));
							//svd.w.at<double>(cv::Point(i, j))=xi_HH_B*wHH_Ga.at<double>(cv::Point(i, j));
							//svd.w.at<double>(cv::Point(i, j)) = wHH_B.at<double>(cv::Point(i, j));
						}
					}
				
					for (int i = 0; i < svd.u.cols; i++)
					{
						for (int j = 0; j < svd.u.rows; j ++)
						{
							svd.u.at<double>(cv::Point(i, j)) = uHH_B.at<double>(cv::Point(i, j));
						}
					}
					for (int i = 0; i < svd.vt.cols; i++)
					{
						for (int j = 0; j < svd.vt.rows; j ++)
						{
							svd.vt.at<double>(cv::Point(i, j)) = vtHH_B.at<double>(cv::Point(i, j));
						}
					}

					Mat HH1_ASVD_B = svd.u*Mat::diag(svd.w)*svd.vt;
				
					//===================
					for (int i = 0; i < svd.w.cols; i++)
					{
						for (int j = 0; j < svd.w.rows; j ++)
						{
							svd.w.at<double>(cv::Point(i, j))=xi_HH_G*wHH_G.at<double>(cv::Point(i, j));
							//svd.w.at<double>(cv::Point(i, j))=xi_HH_G*wHH_Ga.at<double>(cv::Point(i, j));
							//svd.w.at<double>(cv::Point(i, j)) = wHH_G.at<double>(cv::Point(i, j));
						}
					}
				
					for (int i = 0; i < svd.u.cols; i++)
					{
						for (int j = 0; j < svd.u.rows; j ++)
						{
							svd.u.at<double>(cv::Point(i, j)) = uHH_G.at<double>(cv::Point(i, j));
						}
					}
					for (int i = 0; i < svd.vt.cols; i++)
					{
						for (int j = 0; j < svd.vt.rows; j ++)
						{
							svd.vt.at<double>(cv::Point(i, j)) = vtHH_G.at<double>(cv::Point(i, j));
						}
					}
				
					Mat HH1_ASVD_G = svd.u*Mat::diag(svd.w)*svd.vt;
				
					//==========================================
					for (int i = 0; i < svd.w.cols; i++)
					{
						for (int j = 0; j < svd.w.rows; j ++)
						{
							svd.w.at<double>(cv::Point(i, j))=xi_HH_R*wHH_R.at<double>(cv::Point(i, j));
							//svd.w.at<double>(cv::Point(i, j))=xi_HH_R*wHH_Ga.at<double>(cv::Point(i, j));
							//svd.w.at<double>(cv::Point(i, j)) = wHH_R.at<double>(cv::Point(i, j));
						}
					}
				
					for (int i = 0; i < svd.u.cols; i++)
					{
						for (int j = 0; j < svd.u.rows; j ++)
						{
							svd.u.at<double>(cv::Point(i, j)) = uHH_R.at<double>(cv::Point(i, j));
						}
					}
					for (int i = 0; i < svd.vt.cols; i++)
					{
						for (int j = 0; j < svd.vt.rows; j ++)
						{
							svd.vt.at<double>(cv::Point(i, j)) = vtHH_R.at<double>(cv::Point(i, j));
						}
					}

					Mat HH1_ASVD_R = svd.u*Mat::diag(svd.w)*svd.vt;
				
					//====================================================================================================================//	
			
					// Return values after change by ASVDW
				
					for (int k = 0; k < rows_1*cols_1; k++)
					{
						int i = k/cols_1; // thuong->dong
						int j = k%cols_1; // du->cot
						output_ASVD_B[k] = LL1_ASVD_B.at<double>(cv::Point(j, i));
						output_ASVD_G[k] = LL1_ASVD_G.at<double>(cv::Point(j, i));
						output_ASVD_R[k] = LL1_ASVD_R.at<double>(cv::Point(j, i));
					}
				
					for (int k = rows_1*cols_1; k < 2*rows_1*cols_1; k++)
					{
						int k1 = k - rows_1*cols_1;
						int i = k1/cols_1; // thuong->dong
						int j = k1%cols_1; // du->cot
						output_ASVD_B[k] = HL1_ASVD_B.at<double>(cv::Point(j, i));
						output_ASVD_G[k] = HL1_ASVD_G.at<double>(cv::Point(j, i));
						output_ASVD_R[k] = HL1_ASVD_R.at<double>(cv::Point(j, i));
						/*
						////Filter
						if (HL1_ASVD_B.at<double>(cv::Point(j, i))>=0) {output_ASVD_B[k] = HL1_ASVD_B.at<double>(cv::Point(j, i));} else {output_ASVD_B[k]=0;}
						if (HL1_ASVD_G.at<double>(cv::Point(j, i))>=0) {output_ASVD_G[k] = HL1_ASVD_G.at<double>(cv::Point(j, i));} else {output_ASVD_G[k]=0;}
						if (HL1_ASVD_R.at<double>(cv::Point(j, i))>=0) {output_ASVD_R[k] = HL1_ASVD_R.at<double>(cv::Point(j, i));} else {output_ASVD_R[k]=0;}
						*/
					}
				
					for (int k = 2*rows_1*cols_1; k < 3*rows_1*cols_1; k++)
					{
						int k1 = k - 2*rows_1*cols_1;
						int i = k1/cols_1; // thuong->dong
						int j = k1%cols_1; // du->cot
						output_ASVD_B[k] = LH1_ASVD_B.at<double>(cv::Point(j, i));
						output_ASVD_G[k] = LH1_ASVD_G.at<double>(cv::Point(j, i));
						output_ASVD_R[k] = LH1_ASVD_R.at<double>(cv::Point(j, i));
						/*
						////Filter
						if (LH1_ASVD_B.at<double>(cv::Point(j, i))>=0) {output_ASVD_B[k] = LH1_ASVD_B.at<double>(cv::Point(j, i));} else {output_ASVD_B[k]=0;}
						if (LH1_ASVD_G.at<double>(cv::Point(j, i))>=0) {output_ASVD_G[k] = LH1_ASVD_G.at<double>(cv::Point(j, i));} else {output_ASVD_G[k]=0;}
						if (LH1_ASVD_R.at<double>(cv::Point(j, i))>=0) {output_ASVD_R[k] = LH1_ASVD_R.at<double>(cv::Point(j, i));} else {output_ASVD_R[k]=0;}
						*/
					}
					for (int k = 3*rows_1*cols_1; k < 4*rows_1*cols_1; k++)
					{
						int k1 = k - 3*rows_1*cols_1;
						int i = k1/cols_1; // thuong->dong
						int j = k1%cols_1; // du->cot
						output_ASVD_B[k] = HH1_ASVD_B.at<double>(cv::Point(j, i));
						output_ASVD_G[k] = HH1_ASVD_G.at<double>(cv::Point(j, i));
						output_ASVD_R[k] = HH1_ASVD_R.at<double>(cv::Point(j, i));
						/*
						////Filter
						if (HH1_ASVD_B.at<double>(cv::Point(j, i))>=0) {output_ASVD_B[k] = HH1_ASVD_B.at<double>(cv::Point(j, i));}else {output_ASVD_B[k]=0;}
						if (HH1_ASVD_G.at<double>(cv::Point(j, i))>=0) {output_ASVD_G[k] = HH1_ASVD_G.at<double>(cv::Point(j, i));}else {output_ASVD_G[k]=0;}
						if (HH1_ASVD_R.at<double>(cv::Point(j, i))>=0) {output_ASVD_R[k] = HH1_ASVD_R.at<double>(cv::Point(j, i));}else {output_ASVD_R[k]=0;}
						//*/
				
					}
				
					// DWT images after ASVD

					vector<vector< double> > dwtdisp_B_ASVD(rows_n, vector<double>(cols_n));
					vector<vector< double> > dwtdisp_G_ASVD(rows_n, vector<double>(cols_n));
					vector<vector< double> > dwtdisp_R_ASVD(rows_n, vector<double>(cols_n));
				
					dispDWT(output_ASVD_B,dwtdisp_B_ASVD, length_B ,length2_B, J);
					dispDWT(output_ASVD_G,dwtdisp_G_ASVD, length_B ,length2_B, J);
					dispDWT(output_ASVD_R,dwtdisp_R_ASVD, length_B ,length2_B, J);

					vector<vector<double> >  dwt_output_B_ASVD= dwtdisp_B_ASVD;
					vector<vector<double> >  dwt_output_G_ASVD= dwtdisp_G_ASVD;
					vector<vector<double> >  dwt_output_R_ASVD= dwtdisp_R_ASVD;
					double max_B_ASVD, max_G_ASVD, max_R_ASVD;
					max_array(dwt_output_B,max_B_ASVD);// max value is needed to take care of overflow
					max_array(dwt_output_G,max_G_ASVD);
					max_array(dwt_output_R,max_R_ASVD);
				
					cv::Mat img_dwt_ASVD = Mat::zeros(rows_n,cols_n, img_src.type());
				
					// dwt_hold is created to hold the dwt output as further operations need to be
					// carried out on dwt_output in order to display scaled images.
					vector<vector<double> > dwt_hold_B_ASVD(rows_n, vector<double>( cols_n));
					vector<vector<double> > dwt_hold_G_ASVD(rows_n, vector<double>( cols_n));
					vector<vector<double> > dwt_hold_R_ASVD(rows_n, vector<double>( cols_n));
					dwt_hold_B_ASVD = dwt_output_B_ASVD;
					dwt_hold_G_ASVD = dwt_output_G_ASVD;
					dwt_hold_R_ASVD = dwt_output_R_ASVD;
					// Setting coefficients of created image to the scaled DWT output values
					for (int i = 0; i < img_dwt_ASVD.rows; i++ ) 
					{
						for (int j = 0; j < img_dwt_ASVD.cols; j++ )
						{
							if ( dwt_output_B_ASVD[i][j] <= 0.0)
							{
								dwt_output_B_ASVD[i][j] = 0.0;
							}
							if ( dwt_output_G_ASVD[i][j] <= 0.0)
							{
								dwt_output_G_ASVD[i][j] = 0.0;
							}
							if ( dwt_output_R_ASVD[i][j] <= 0.0)
							{
								dwt_output_R_ASVD[i][j] = 0.0;
							}

							if ( i <= (length2_B[0]) && j <= (length2_B[1]) ) 
							{
							
								img_dwt_ASVD.at<Vec3f>(cv::Point(j, i))[0] = (float) ( (dwt_output_B_ASVD[i][j] / max_B) * 255.0);
								img_dwt_ASVD.at<Vec3f>(cv::Point(j, i))[1] = (float) ( (dwt_output_G_ASVD[i][j] / max_G) * 255.0);
								img_dwt_ASVD.at<Vec3f>(cv::Point(j, i))[2] = (float) ( (dwt_output_R_ASVD[i][j] / max_R) * 255.0);
							} 
							else 
							{
								img_dwt_ASVD.at<Vec3f>(cv::Point(j, i))[0] = (float) (dwt_output_B_ASVD[i][j]);
								img_dwt_ASVD.at<Vec3f>(cv::Point(j, i))[1] = (float) (dwt_output_G_ASVD[i][j]);
								img_dwt_ASVD.at<Vec3f>(cv::Point(j, i))[2] = (float) (dwt_output_R_ASVD[i][j]);
							}
						}
					}
					/////////////////////
					// Finding IDWT

					vector<vector<double> > idwt_output_ASVD_B(rows, vector<double>(cols));
					vector<vector<double> > idwt_output_ASVD_G(rows, vector<double>(cols));
					vector<vector<double> > idwt_output_ASVD_R(rows, vector<double>(cols));

					idwt_2d(output_ASVD_B,flag, nm, idwt_output_ASVD_B,length_B);
					idwt_2d(output_ASVD_G,flag, nm, idwt_output_ASVD_G,length_G);
					idwt_2d(output_ASVD_R,flag, nm, idwt_output_ASVD_R,length_R);
				TIMER_STOP
					
					
					
					//Result image
					
					Mat img_ASVD = Mat(img_src.size(),img_src.type());
					for (int i=0; i < rows; i++) 
					{
						for (int j =0; j < cols; j++)
						{	
						
							img_ASVD.at<Vec3f>(cv::Point(j, i))[0] = idwt_output_ASVD_B[i][j];
							img_ASVD.at<Vec3f>(cv::Point(j, i))[1] = idwt_output_ASVD_G[i][j];
							img_ASVD.at<Vec3f>(cv::Point(j, i))[2] = idwt_output_ASVD_R[i][j];
							
						}
					}


					
					
					//normalize(img_ASVD, img_ASVD, 0, 255, NORM_MINMAX, CV_8UC3);
					img_ASVD.convertTo(img_ASVD, CV_8UC3);
					imshow("",img_ASVD);
					char* savename;
					savename = (char*)Marshal::StringToHGlobalAnsi(pathfolder + "\\" + (step+1).ToString()+".bmp").ToPointer();
					imwrite( savename, img_ASVD);
					
					/// Display rerults

					int row_s = (img.rows/4)*4;
					int col_s = (img.cols/4)*4;
					Mat img_s = Mat(row_s, col_s, img.type());
					resize(img, img_s, img_s.size(), 0, 0, INTER_LINEAR);
					pB_ori->Image  = gcnew    
					System::Drawing::Bitmap(img_s.size().width,img_s.size().height,img_s.step,
					System::Drawing::Imaging::PixelFormat::Format24bppRgb,(System::IntPtr) img_s.data);
					pB_ori->Refresh();

					img_dwt.convertTo(img_dwt, CV_8UC3);
					Mat img_dwt_disp = Mat(row_s, col_s, img.type());
					resize(img_dwt, img_dwt_disp, img_dwt_disp.size(), 0, 0, INTER_LINEAR);
					
					
					
					pB_wav->Image  = gcnew    
					System::Drawing::Bitmap(img_dwt_disp.size().width,img_dwt_disp.size().height,img_dwt_disp.step,
					System::Drawing::Imaging::PixelFormat::Format24bppRgb,(System::IntPtr) img_dwt_disp.data);
					pB_wav->Refresh();

					

					
					img_dwt_ASVD.convertTo(img_dwt_ASVD, CV_8UC3);
					Mat img_dwt_ASVD_disp = Mat(row_s, col_s, img.type());
					resize(img_dwt_ASVD, img_dwt_ASVD_disp, img_dwt_ASVD_disp.size(), 0, 0, INTER_LINEAR);

					pB_wav_ASVD->Image  = gcnew    
					System::Drawing::Bitmap(img_dwt_ASVD_disp.size().width,img_dwt_ASVD_disp.size().height,img_dwt_ASVD_disp.step,
					System::Drawing::Imaging::PixelFormat::Format24bppRgb,(System::IntPtr) img_dwt_ASVD_disp.data);
					pB_wav_ASVD->Refresh();


					Mat img_ASVD_s = Mat(row_s, col_s, img.type());
					resize(img_ASVD, img_ASVD_s, img_ASVD_s.size(), 0, 0, INTER_LINEAR);
					pB_dst_ASVDW->Image  = gcnew    
					System::Drawing::Bitmap(img_ASVD_s.size().width,img_ASVD_s.size().height,img_ASVD_s.step,
					System::Drawing::Imaging::PixelFormat::Format24bppRgb,(System::IntPtr) img_ASVD_s.data);
					pB_dst_ASVDW->Refresh();

					Mat histImage = drawhistogram(img);
					pB_hist_src->Image  = gcnew    
					System::Drawing::Bitmap(histImage.size().width,histImage.size().height,histImage.step,
					System::Drawing::Imaging::PixelFormat::Format24bppRgb,(System::IntPtr) histImage.data);
					pB_hist_src->Refresh();

					Mat histImage_dst = drawhistogram(img_ASVD);
					pB_hist_dst->Image  = gcnew    
					System::Drawing::Bitmap(histImage_dst.size().width,histImage_dst.size().height,histImage_dst.step,
					System::Drawing::Imaging::PixelFormat::Format24bppRgb,(System::IntPtr) histImage_dst.data);
					pB_hist_dst->Refresh();

					txt_xi_HH_B->Text = xi_HH_B.ToString();txt_xi_HH_B->Refresh();
					txt_xi_HH_G->Text = xi_HH_G.ToString();txt_xi_HH_G->Refresh();
					txt_xi_HH_R->Text = xi_HH_R.ToString();txt_xi_HH_R->Refresh();
					
					txt_xi_HL_B->Text = xi_HL_B.ToString();txt_xi_HL_B->Refresh();
					txt_xi_HL_G->Text = xi_HL_G.ToString();txt_xi_HL_G->Refresh();
					txt_xi_HL_R->Text = xi_HL_R.ToString();txt_xi_HL_R->Refresh();
					
					
					txt_xi_LH_B->Text = xi_LH_B.ToString();txt_xi_LH_B->Refresh();
					txt_xi_LH_G->Text = xi_LH_G.ToString();txt_xi_LH_G->Refresh();
					txt_xi_LH_R->Text = xi_LH_R.ToString();txt_xi_LH_R->Refresh();

					txt_xi_LL_B->Text = xi_LL_G.ToString();txt_xi_LL_B->Refresh();
					txt_xi_LL_G->Text = xi_LL_B.ToString();txt_xi_LL_G->Refresh();
					txt_xi_LL_R->Text = xi_LL_R.ToString();txt_xi_LL_R->Refresh();



					txt_pLP_img->Text = pLP_img.ToString();txt_pLP_img->Refresh();
					txt_pNP_img->Text = pNP_img.ToString();txt_pNP_img->Refresh();
					txt_pHP_img->Text = pHP_img.ToString();txt_pHP_img->Refresh();
					txt_meanGa->Text = mi.ToString();txt_meanGa->Refresh();

				

					step++;
				}

			}
private: System::Void xi_LL_B_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void textBox2_TextChanged(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void cB_dataname_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			 txt_savefolder->Text= cB_dataname->Text;
		 }
private: System::Void label28_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void txt_savefolder_TextChanged(System::Object^  sender, System::EventArgs^  e) {
		 }
};
}
