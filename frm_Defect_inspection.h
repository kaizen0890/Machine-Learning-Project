#pragma once

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/contrib/contrib.hpp"
#include <iostream>
#include <msclr/marshal_cppstd.h>
#include "math.h"
//------------------------------------------------------------------

using namespace System::Runtime::InteropServices;
using namespace cv;
using namespace std;
using namespace msclr::interop;

// Compute the Time
#define TIMER_INIT LARGE_INTEGER frequency;LARGE_INTEGER t1,t2;double elapsedTime;QueryPerformanceFrequency(&frequency);

// Use to start the performance timer 
#define TIMER_START QueryPerformanceCounter(&t1);

// Use to stop the performance timer and output the result to the standard stream. Less verbose than \c TIMER_STOP_VERBOSE 
#define TIMER_STOP QueryPerformanceCounter(&t2);elapsedTime=(float)(t2.QuadPart-t1.QuadPart)/frequency.QuadPart;//std::wcout<<elapsedTime<<L" sec"<<endl;
//End of Compute the Time

//static int mode=0;
//static double process_time1=0,process_time2=0;
static int PixelLimit=0,EnergyThreshold=0;
static int otsu_hist[256];

static int roi_cut=10;

static int InputH,InputW; 
static int RoiH,RoiW; 
static int ExistFieldPosition_deleteH; 
static int Defect_InputH,Defect_InputW; 
//-------------------------------------------------------------------
static int **R_matrix=NULL,**G_matrix=NULL,**B_matrix=NULL,**Gray_matrix=NULL; //Original
static int **otsu_Gray_matrix; //Otsu
static int **roi_matrix=NULL,**energy_matrix=NULL;
static int **roi_R_matrix=NULL,**roi_G_matrix=NULL,**roi_B_matrix=NULL; //ROI
static int **Defect_R_matrix=NULL,**Defect_G_matrix=NULL,**Defect_B_matrix=NULL,**Defect_Gray_matrix=NULL; //Defect
//------------------------------------------------------------------
static Mat *iii;
static int roi_image_postion[2][2];    
static int **ExistFieldPosition=NULL;  
static int ExistFieldCounter=0;
static int RoiMinH=0,RoiMaxH=0,RoiMinW=0,RoiMaxW=0;
#define ClassificationIndexScale 0.5

//=================================================================================================
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
//=================================================================================================
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
static void Matrix2DDelete(int **DeleteMatrix,int bmpH)
{
                for(int y=0;y<=bmpH-1;y++)
                        delete [] DeleteMatrix[y];
                delete [] DeleteMatrix;
}
//=====================================================================================================

namespace Phone_monitor {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for frm_Defect_inspection
	/// </summary>
	public ref class frm_Defect_inspection : public System::Windows::Forms::Form
	{
	public:
		frm_Defect_inspection(void)
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
		~frm_Defect_inspection()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  btn_start;



	private: System::Windows::Forms::OpenFileDialog^  openFileDialog;




	private: System::Windows::Forms::PictureBox^  pB_orig;
	private: System::Windows::Forms::PictureBox^  pB_roi;
	private: System::Windows::Forms::PictureBox^  pB_otsu_hist_hor;



	private: System::Windows::Forms::PictureBox^  pB_otsu;
	private: System::Windows::Forms::PictureBox^  pB_otsu_hist_ver;

	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::GroupBox^  groupBox2;
	private: System::Windows::Forms::GroupBox^  groupBox3;
	private: System::Windows::Forms::PictureBox^  pB_ROIcut;
	private: System::Windows::Forms::GroupBox^  groupBox4;
	private: System::Windows::Forms::GroupBox^  groupBox5;
	private: System::Windows::Forms::PictureBox^  pB_en_hor;

	private: System::Windows::Forms::PictureBox^  pB_en_ver;




	private: System::Windows::Forms::PictureBox^  pB_energy;
	private: System::Windows::Forms::GroupBox^  groupBox6;
	private: System::Windows::Forms::CheckBox^  cB_energy;
	private: System::Windows::Forms::CheckBox^  cB_all;
	private: System::Windows::Forms::CheckBox^  cB_ROIcutsign;
	private: System::Windows::Forms::CheckBox^  cB_ROIpaint;
	private: System::Windows::Forms::CheckBox^  cB_ROIcut;
	private: System::Windows::Forms::CheckBox^  cB_otsu;
	private: System::Windows::Forms::GroupBox^  groupBox7;
	private: System::Windows::Forms::TextBox^  txt_hl;

	private: System::Windows::Forms::TextBox^  txt_w;

	private: System::Windows::Forms::TextBox^  txt_h;

	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::GroupBox^  groupBox8;
	private: System::Windows::Forms::ListBox^  lB_defects;
	private: System::Windows::Forms::GroupBox^  groupBox9;
	private: System::Windows::Forms::TextBox^  txt_SD;

	private: System::Windows::Forms::TextBox^  txt_pixellimit;

	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::TextBox^  txt_energythreshold;

	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::GroupBox^  groupBox10;
	private: System::Windows::Forms::GroupBox^  groupBox12;
	private: System::Windows::Forms::PictureBox^  pB_defectS;

	private: System::Windows::Forms::GroupBox^  groupBox11;
	private: System::Windows::Forms::PictureBox^  pB_defect;

	private: System::Windows::Forms::GroupBox^  groupBox13;
	private: System::Windows::Forms::TextBox^  txt_B;

	private: System::Windows::Forms::TextBox^  txt_G;

	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::TextBox^  txt_R;

	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::TextBox^  txt_Gray;

	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::GroupBox^  groupBox14;
	private: System::Windows::Forms::TextBox^  txt_BR;

	private: System::Windows::Forms::TextBox^  txt_GR;

	private: System::Windows::Forms::Label^  label11;
private: System::Windows::Forms::TextBox^  txt_RR;

	private: System::Windows::Forms::Label^  label12;
private: System::Windows::Forms::TextBox^  txt_GrayR;


	private: System::Windows::Forms::Label^  label13;
	private: System::Windows::Forms::Label^  label14;
	private: System::Windows::Forms::GroupBox^  groupBox15;
	private: System::Windows::Forms::GroupBox^  groupBox16;
private: System::Windows::Forms::TextBox^  txt_Sg;

	private: System::Windows::Forms::Button^  btn_clear;
	private: System::Windows::Forms::RadioButton^  rB_ver;
	private: System::Windows::Forms::RadioButton^  rB_hor;
private: System::Windows::Forms::Button^  btn_clear1;
private: System::Windows::Forms::GroupBox^  groupBox17;
private: System::Windows::Forms::TextBox^  txt_time;
private: System::Windows::Forms::GroupBox^  groupBox19;
private: System::Windows::Forms::GroupBox^  groupBox18;



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
			this->btn_start = (gcnew System::Windows::Forms::Button());
			this->openFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());
			this->pB_orig = (gcnew System::Windows::Forms::PictureBox());
			this->pB_roi = (gcnew System::Windows::Forms::PictureBox());
			this->pB_otsu_hist_hor = (gcnew System::Windows::Forms::PictureBox());
			this->pB_otsu = (gcnew System::Windows::Forms::PictureBox());
			this->pB_otsu_hist_ver = (gcnew System::Windows::Forms::PictureBox());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
			this->pB_ROIcut = (gcnew System::Windows::Forms::PictureBox());
			this->groupBox4 = (gcnew System::Windows::Forms::GroupBox());
			this->groupBox5 = (gcnew System::Windows::Forms::GroupBox());
			this->pB_en_hor = (gcnew System::Windows::Forms::PictureBox());
			this->pB_en_ver = (gcnew System::Windows::Forms::PictureBox());
			this->pB_energy = (gcnew System::Windows::Forms::PictureBox());
			this->groupBox6 = (gcnew System::Windows::Forms::GroupBox());
			this->cB_energy = (gcnew System::Windows::Forms::CheckBox());
			this->cB_all = (gcnew System::Windows::Forms::CheckBox());
			this->cB_ROIcutsign = (gcnew System::Windows::Forms::CheckBox());
			this->cB_ROIpaint = (gcnew System::Windows::Forms::CheckBox());
			this->cB_ROIcut = (gcnew System::Windows::Forms::CheckBox());
			this->cB_otsu = (gcnew System::Windows::Forms::CheckBox());
			this->groupBox7 = (gcnew System::Windows::Forms::GroupBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->txt_hl = (gcnew System::Windows::Forms::TextBox());
			this->txt_w = (gcnew System::Windows::Forms::TextBox());
			this->txt_h = (gcnew System::Windows::Forms::TextBox());
			this->groupBox8 = (gcnew System::Windows::Forms::GroupBox());
			this->lB_defects = (gcnew System::Windows::Forms::ListBox());
			this->groupBox9 = (gcnew System::Windows::Forms::GroupBox());
			this->groupBox19 = (gcnew System::Windows::Forms::GroupBox());
			this->groupBox18 = (gcnew System::Windows::Forms::GroupBox());
			this->txt_SD = (gcnew System::Windows::Forms::TextBox());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->txt_pixellimit = (gcnew System::Windows::Forms::TextBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->txt_energythreshold = (gcnew System::Windows::Forms::TextBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->groupBox10 = (gcnew System::Windows::Forms::GroupBox());
			this->groupBox12 = (gcnew System::Windows::Forms::GroupBox());
			this->pB_defectS = (gcnew System::Windows::Forms::PictureBox());
			this->groupBox11 = (gcnew System::Windows::Forms::GroupBox());
			this->pB_defect = (gcnew System::Windows::Forms::PictureBox());
			this->groupBox13 = (gcnew System::Windows::Forms::GroupBox());
			this->txt_B = (gcnew System::Windows::Forms::TextBox());
			this->txt_G = (gcnew System::Windows::Forms::TextBox());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->txt_R = (gcnew System::Windows::Forms::TextBox());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->txt_Gray = (gcnew System::Windows::Forms::TextBox());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->groupBox14 = (gcnew System::Windows::Forms::GroupBox());
			this->txt_BR = (gcnew System::Windows::Forms::TextBox());
			this->txt_GR = (gcnew System::Windows::Forms::TextBox());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->txt_RR = (gcnew System::Windows::Forms::TextBox());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->txt_GrayR = (gcnew System::Windows::Forms::TextBox());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->groupBox15 = (gcnew System::Windows::Forms::GroupBox());
			this->btn_clear1 = (gcnew System::Windows::Forms::Button());
			this->groupBox16 = (gcnew System::Windows::Forms::GroupBox());
			this->txt_Sg = (gcnew System::Windows::Forms::TextBox());
			this->btn_clear = (gcnew System::Windows::Forms::Button());
			this->rB_ver = (gcnew System::Windows::Forms::RadioButton());
			this->rB_hor = (gcnew System::Windows::Forms::RadioButton());
			this->groupBox17 = (gcnew System::Windows::Forms::GroupBox());
			this->txt_time = (gcnew System::Windows::Forms::TextBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_orig))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_roi))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_otsu_hist_hor))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_otsu))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_otsu_hist_ver))->BeginInit();
			this->groupBox1->SuspendLayout();
			this->groupBox2->SuspendLayout();
			this->groupBox3->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_ROIcut))->BeginInit();
			this->groupBox4->SuspendLayout();
			this->groupBox5->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_en_hor))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_en_ver))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_energy))->BeginInit();
			this->groupBox6->SuspendLayout();
			this->groupBox7->SuspendLayout();
			this->groupBox8->SuspendLayout();
			this->groupBox9->SuspendLayout();
			this->groupBox18->SuspendLayout();
			this->groupBox10->SuspendLayout();
			this->groupBox12->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_defectS))->BeginInit();
			this->groupBox11->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_defect))->BeginInit();
			this->groupBox13->SuspendLayout();
			this->groupBox14->SuspendLayout();
			this->groupBox15->SuspendLayout();
			this->groupBox16->SuspendLayout();
			this->groupBox17->SuspendLayout();
			this->SuspendLayout();
			// 
			// btn_start
			// 
			this->btn_start->Location = System::Drawing::Point(26, 19);
			this->btn_start->Name = L"btn_start";
			this->btn_start->Size = System::Drawing::Size(121, 53);
			this->btn_start->TabIndex = 0;
			this->btn_start->Text = L"Start";
			this->btn_start->UseVisualStyleBackColor = true;
			this->btn_start->Click += gcnew System::EventHandler(this, &frm_Defect_inspection::btn_start_Click);
			// 
			// openFileDialog
			// 
			this->openFileDialog->FileName = L"openFileDialog";
			// 
			// pB_orig
			// 
			this->pB_orig->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pB_orig->Location = System::Drawing::Point(6, 17);
			this->pB_orig->Name = L"pB_orig";
			this->pB_orig->Size = System::Drawing::Size(260, 196);
			this->pB_orig->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pB_orig->TabIndex = 1;
			this->pB_orig->TabStop = false;
			this->pB_orig->Click += gcnew System::EventHandler(this, &frm_Defect_inspection::pB_orig_Click);
			// 
			// pB_roi
			// 
			this->pB_roi->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pB_roi->Location = System::Drawing::Point(10, 16);
			this->pB_roi->Name = L"pB_roi";
			this->pB_roi->Size = System::Drawing::Size(260, 196);
			this->pB_roi->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pB_roi->TabIndex = 1;
			this->pB_roi->TabStop = false;
			// 
			// pB_otsu_hist_hor
			// 
			this->pB_otsu_hist_hor->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pB_otsu_hist_hor->Location = System::Drawing::Point(6, 218);
			this->pB_otsu_hist_hor->Name = L"pB_otsu_hist_hor";
			this->pB_otsu_hist_hor->Size = System::Drawing::Size(260, 56);
			this->pB_otsu_hist_hor->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pB_otsu_hist_hor->TabIndex = 1;
			this->pB_otsu_hist_hor->TabStop = false;
			this->pB_otsu_hist_hor->Click += gcnew System::EventHandler(this, &frm_Defect_inspection::B_otsu_hist_ver_Click);
			// 
			// pB_otsu
			// 
			this->pB_otsu->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pB_otsu->Location = System::Drawing::Point(6, 16);
			this->pB_otsu->Name = L"pB_otsu";
			this->pB_otsu->Size = System::Drawing::Size(260, 196);
			this->pB_otsu->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pB_otsu->TabIndex = 1;
			this->pB_otsu->TabStop = false;
			this->pB_otsu->Click += gcnew System::EventHandler(this, &frm_Defect_inspection::pB_otsu_Click);
			// 
			// pB_otsu_hist_ver
			// 
			this->pB_otsu_hist_ver->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pB_otsu_hist_ver->Location = System::Drawing::Point(272, 16);
			this->pB_otsu_hist_ver->Name = L"pB_otsu_hist_ver";
			this->pB_otsu_hist_ver->Size = System::Drawing::Size(68, 196);
			this->pB_otsu_hist_ver->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pB_otsu_hist_ver->TabIndex = 1;
			this->pB_otsu_hist_ver->TabStop = false;
			// 
			// groupBox1
			// 
			this->groupBox1->BackColor = System::Drawing::SystemColors::Control;
			this->groupBox1->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->groupBox1->Controls->Add(this->pB_orig);
			this->groupBox1->Location = System::Drawing::Point(17, 91);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(274, 218);
			this->groupBox1->TabIndex = 2;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Original Image";
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->pB_otsu_hist_hor);
			this->groupBox2->Controls->Add(this->pB_otsu_hist_ver);
			this->groupBox2->Controls->Add(this->pB_otsu);
			this->groupBox2->Location = System::Drawing::Point(304, 91);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(349, 283);
			this->groupBox2->TabIndex = 3;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"Otsu Image";
			// 
			// groupBox3
			// 
			this->groupBox3->Controls->Add(this->pB_roi);
			this->groupBox3->Location = System::Drawing::Point(668, 91);
			this->groupBox3->Name = L"groupBox3";
			this->groupBox3->Size = System::Drawing::Size(276, 218);
			this->groupBox3->TabIndex = 4;
			this->groupBox3->TabStop = false;
			this->groupBox3->Text = L"ROImark";
			// 
			// pB_ROIcut
			// 
			this->pB_ROIcut->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pB_ROIcut->Location = System::Drawing::Point(6, 16);
			this->pB_ROIcut->Name = L"pB_ROIcut";
			this->pB_ROIcut->Size = System::Drawing::Size(260, 196);
			this->pB_ROIcut->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pB_ROIcut->TabIndex = 1;
			this->pB_ROIcut->TabStop = false;
			this->pB_ROIcut->Click += gcnew System::EventHandler(this, &frm_Defect_inspection::pB_ROIcut_Click);
			// 
			// groupBox4
			// 
			this->groupBox4->BackColor = System::Drawing::SystemColors::Control;
			this->groupBox4->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->groupBox4->Controls->Add(this->pB_ROIcut);
			this->groupBox4->Location = System::Drawing::Point(17, 391);
			this->groupBox4->Name = L"groupBox4";
			this->groupBox4->Size = System::Drawing::Size(274, 218);
			this->groupBox4->TabIndex = 5;
			this->groupBox4->TabStop = false;
			this->groupBox4->Text = L"ROIcut/ ROIcut mark";
			this->groupBox4->Enter += gcnew System::EventHandler(this, &frm_Defect_inspection::groupBox4_Enter);
			// 
			// groupBox5
			// 
			this->groupBox5->Controls->Add(this->pB_en_hor);
			this->groupBox5->Controls->Add(this->pB_en_ver);
			this->groupBox5->Controls->Add(this->pB_energy);
			this->groupBox5->Location = System::Drawing::Point(304, 391);
			this->groupBox5->Name = L"groupBox5";
			this->groupBox5->Size = System::Drawing::Size(349, 283);
			this->groupBox5->TabIndex = 3;
			this->groupBox5->TabStop = false;
			this->groupBox5->Text = L"Localized energy anlysis";
			// 
			// pB_en_hor
			// 
			this->pB_en_hor->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pB_en_hor->Location = System::Drawing::Point(6, 218);
			this->pB_en_hor->Name = L"pB_en_hor";
			this->pB_en_hor->Size = System::Drawing::Size(260, 56);
			this->pB_en_hor->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pB_en_hor->TabIndex = 1;
			this->pB_en_hor->TabStop = false;
			this->pB_en_hor->Click += gcnew System::EventHandler(this, &frm_Defect_inspection::B_otsu_hist_ver_Click);
			// 
			// pB_en_ver
			// 
			this->pB_en_ver->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pB_en_ver->Location = System::Drawing::Point(272, 16);
			this->pB_en_ver->Name = L"pB_en_ver";
			this->pB_en_ver->Size = System::Drawing::Size(68, 196);
			this->pB_en_ver->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pB_en_ver->TabIndex = 1;
			this->pB_en_ver->TabStop = false;
			// 
			// pB_energy
			// 
			this->pB_energy->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pB_energy->Location = System::Drawing::Point(6, 16);
			this->pB_energy->Name = L"pB_energy";
			this->pB_energy->Size = System::Drawing::Size(260, 196);
			this->pB_energy->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pB_energy->TabIndex = 1;
			this->pB_energy->TabStop = false;
			this->pB_energy->Click += gcnew System::EventHandler(this, &frm_Defect_inspection::pB_energy_Click);
			// 
			// groupBox6
			// 
			this->groupBox6->Controls->Add(this->cB_energy);
			this->groupBox6->Controls->Add(this->cB_all);
			this->groupBox6->Controls->Add(this->cB_ROIcutsign);
			this->groupBox6->Controls->Add(this->cB_ROIpaint);
			this->groupBox6->Controls->Add(this->cB_ROIcut);
			this->groupBox6->Controls->Add(this->cB_otsu);
			this->groupBox6->Location = System::Drawing::Point(983, 14);
			this->groupBox6->Name = L"groupBox6";
			this->groupBox6->Size = System::Drawing::Size(275, 61);
			this->groupBox6->TabIndex = 6;
			this->groupBox6->TabStop = false;
			this->groupBox6->Text = L"Save Pictures";
			this->groupBox6->Enter += gcnew System::EventHandler(this, &frm_Defect_inspection::groupBox6_Enter);
			// 
			// cB_energy
			// 
			this->cB_energy->AutoSize = true;
			this->cB_energy->Location = System::Drawing::Point(94, 36);
			this->cB_energy->Name = L"cB_energy";
			this->cB_energy->Size = System::Drawing::Size(59, 17);
			this->cB_energy->TabIndex = 1;
			this->cB_energy->Text = L"Energy";
			this->cB_energy->UseVisualStyleBackColor = true;
			// 
			// cB_all
			// 
			this->cB_all->AutoSize = true;
			this->cB_all->Location = System::Drawing::Point(184, 36);
			this->cB_all->Name = L"cB_all";
			this->cB_all->Size = System::Drawing::Size(37, 17);
			this->cB_all->TabIndex = 1;
			this->cB_all->Text = L"All";
			this->cB_all->UseVisualStyleBackColor = true;
			this->cB_all->CheckedChanged += gcnew System::EventHandler(this, &frm_Defect_inspection::cB_all_CheckedChanged);
			// 
			// cB_ROIcutsign
			// 
			this->cB_ROIcutsign->AutoSize = true;
			this->cB_ROIcutsign->Location = System::Drawing::Point(184, 15);
			this->cB_ROIcutsign->Name = L"cB_ROIcutsign";
			this->cB_ROIcutsign->Size = System::Drawing::Size(86, 17);
			this->cB_ROIcutsign->TabIndex = 1;
			this->cB_ROIcutsign->Text = L"ROIcut mark";
			this->cB_ROIcutsign->UseVisualStyleBackColor = true;
			// 
			// cB_ROIpaint
			// 
			this->cB_ROIpaint->AutoSize = true;
			this->cB_ROIpaint->Location = System::Drawing::Point(94, 15);
			this->cB_ROIpaint->Name = L"cB_ROIpaint";
			this->cB_ROIpaint->Size = System::Drawing::Size(68, 17);
			this->cB_ROIpaint->TabIndex = 1;
			this->cB_ROIpaint->Text = L"ROImark";
			this->cB_ROIpaint->UseVisualStyleBackColor = true;
			// 
			// cB_ROIcut
			// 
			this->cB_ROIcut->AutoSize = true;
			this->cB_ROIcut->Location = System::Drawing::Point(16, 38);
			this->cB_ROIcut->Name = L"cB_ROIcut";
			this->cB_ROIcut->Size = System::Drawing::Size(60, 17);
			this->cB_ROIcut->TabIndex = 1;
			this->cB_ROIcut->Text = L"ROIcut";
			this->cB_ROIcut->UseVisualStyleBackColor = true;
			// 
			// cB_otsu
			// 
			this->cB_otsu->AutoSize = true;
			this->cB_otsu->Location = System::Drawing::Point(16, 15);
			this->cB_otsu->Name = L"cB_otsu";
			this->cB_otsu->Size = System::Drawing::Size(48, 17);
			this->cB_otsu->TabIndex = 0;
			this->cB_otsu->Text = L"Otsu";
			this->cB_otsu->UseVisualStyleBackColor = true;
			// 
			// groupBox7
			// 
			this->groupBox7->Controls->Add(this->label3);
			this->groupBox7->Controls->Add(this->label2);
			this->groupBox7->Controls->Add(this->label1);
			this->groupBox7->Controls->Add(this->txt_hl);
			this->groupBox7->Controls->Add(this->txt_w);
			this->groupBox7->Controls->Add(this->txt_h);
			this->groupBox7->Location = System::Drawing::Point(959, 223);
			this->groupBox7->Name = L"groupBox7";
			this->groupBox7->Size = System::Drawing::Size(272, 104);
			this->groupBox7->TabIndex = 7;
			this->groupBox7->TabStop = false;
			this->groupBox7->Text = L"Sub-regions Information";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(11, 74);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(53, 13);
			this->label3->TabIndex = 1;
			this->label3->Text = L"Highlights";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(11, 48);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(35, 13);
			this->label2->TabIndex = 1;
			this->label2->Text = L"Width";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(11, 25);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(32, 13);
			this->label1->TabIndex = 1;
			this->label1->Text = L"Hight";
			// 
			// txt_hl
			// 
			this->txt_hl->Location = System::Drawing::Point(114, 74);
			this->txt_hl->Name = L"txt_hl";
			this->txt_hl->Size = System::Drawing::Size(149, 20);
			this->txt_hl->TabIndex = 0;
			// 
			// txt_w
			// 
			this->txt_w->Location = System::Drawing::Point(114, 48);
			this->txt_w->Name = L"txt_w";
			this->txt_w->Size = System::Drawing::Size(149, 20);
			this->txt_w->TabIndex = 0;
			this->txt_w->TextChanged += gcnew System::EventHandler(this, &frm_Defect_inspection::textBox2_TextChanged);
			// 
			// txt_h
			// 
			this->txt_h->Location = System::Drawing::Point(114, 22);
			this->txt_h->Name = L"txt_h";
			this->txt_h->Size = System::Drawing::Size(149, 20);
			this->txt_h->TabIndex = 0;
			this->txt_h->TextChanged += gcnew System::EventHandler(this, &frm_Defect_inspection::txt_h_TextChanged);
			// 
			// groupBox8
			// 
			this->groupBox8->Controls->Add(this->lB_defects);
			this->groupBox8->Location = System::Drawing::Point(674, 391);
			this->groupBox8->Name = L"groupBox8";
			this->groupBox8->Size = System::Drawing::Size(269, 283);
			this->groupBox8->TabIndex = 8;
			this->groupBox8->TabStop = false;
			this->groupBox8->Text = L"List of Defects";
			this->groupBox8->Enter += gcnew System::EventHandler(this, &frm_Defect_inspection::groupBox8_Enter);
			// 
			// lB_defects
			// 
			this->lB_defects->FormattingEnabled = true;
			this->lB_defects->Location = System::Drawing::Point(6, 19);
			this->lB_defects->Name = L"lB_defects";
			this->lB_defects->Size = System::Drawing::Size(254, 251);
			this->lB_defects->TabIndex = 0;
			this->lB_defects->SelectedIndexChanged += gcnew System::EventHandler(this, &frm_Defect_inspection::lB_defects_SelectedIndexChanged);
			// 
			// groupBox9
			// 
			this->groupBox9->Controls->Add(this->groupBox19);
			this->groupBox9->Controls->Add(this->groupBox18);
			this->groupBox9->Controls->Add(this->btn_start);
			this->groupBox9->Controls->Add(this->groupBox6);
			this->groupBox9->Location = System::Drawing::Point(17, 2);
			this->groupBox9->Name = L"groupBox9";
			this->groupBox9->Size = System::Drawing::Size(1278, 83);
			this->groupBox9->TabIndex = 9;
			this->groupBox9->TabStop = false;
			this->groupBox9->Enter += gcnew System::EventHandler(this, &frm_Defect_inspection::groupBox9_Enter);
			// 
			// groupBox19
			// 
			this->groupBox19->Location = System::Drawing::Point(215, 10);
			this->groupBox19->Name = L"groupBox19";
			this->groupBox19->Size = System::Drawing::Size(160, 67);
			this->groupBox19->TabIndex = 8;
			this->groupBox19->TabStop = false;
			this->groupBox19->Text = L"Mode";
			// 
			// groupBox18
			// 
			this->groupBox18->Controls->Add(this->txt_SD);
			this->groupBox18->Controls->Add(this->label6);
			this->groupBox18->Controls->Add(this->txt_pixellimit);
			this->groupBox18->Controls->Add(this->label5);
			this->groupBox18->Controls->Add(this->txt_energythreshold);
			this->groupBox18->Controls->Add(this->label4);
			this->groupBox18->Location = System::Drawing::Point(432, 16);
			this->groupBox18->Name = L"groupBox18";
			this->groupBox18->Size = System::Drawing::Size(480, 59);
			this->groupBox18->TabIndex = 7;
			this->groupBox18->TabStop = false;
			this->groupBox18->Text = L"Control parameters";
			// 
			// txt_SD
			// 
			this->txt_SD->Location = System::Drawing::Point(399, 19);
			this->txt_SD->Name = L"txt_SD";
			this->txt_SD->Size = System::Drawing::Size(63, 20);
			this->txt_SD->TabIndex = 1;
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(322, 23);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(71, 13);
			this->label6->TabIndex = 0;
			this->label6->Text = L"Scratch/Dust";
			this->label6->Click += gcnew System::EventHandler(this, &frm_Defect_inspection::label6_Click);
			// 
			// txt_pixellimit
			// 
			this->txt_pixellimit->Location = System::Drawing::Point(238, 19);
			this->txt_pixellimit->Name = L"txt_pixellimit";
			this->txt_pixellimit->Size = System::Drawing::Size(63, 20);
			this->txt_pixellimit->TabIndex = 1;
			this->txt_pixellimit->TextChanged += gcnew System::EventHandler(this, &frm_Defect_inspection::txt_pixellimit_TextChanged);
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(183, 23);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(49, 13);
			this->label5->TabIndex = 0;
			this->label5->Text = L"Pixel limit";
			this->label5->Click += gcnew System::EventHandler(this, &frm_Defect_inspection::label5_Click);
			// 
			// txt_energythreshold
			// 
			this->txt_energythreshold->Location = System::Drawing::Point(-145, 11);
			this->txt_energythreshold->Name = L"txt_energythreshold";
			this->txt_energythreshold->Size = System::Drawing::Size(63, 20);
			this->txt_energythreshold->TabIndex = 1;
			this->txt_energythreshold->TextChanged += gcnew System::EventHandler(this, &frm_Defect_inspection::txt_energythreshold_TextChanged);
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(6, 23);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(86, 13);
			this->label4->TabIndex = 0;
			this->label4->Text = L"Energy threshold";
			this->label4->Click += gcnew System::EventHandler(this, &frm_Defect_inspection::label4_Click);
			// 
			// groupBox10
			// 
			this->groupBox10->Controls->Add(this->groupBox12);
			this->groupBox10->Controls->Add(this->groupBox11);
			this->groupBox10->Location = System::Drawing::Point(959, 366);
			this->groupBox10->Name = L"groupBox10";
			this->groupBox10->Size = System::Drawing::Size(336, 154);
			this->groupBox10->TabIndex = 10;
			this->groupBox10->TabStop = false;
			this->groupBox10->Text = L"Defect analysis";
			// 
			// groupBox12
			// 
			this->groupBox12->Controls->Add(this->pB_defectS);
			this->groupBox12->Location = System::Drawing::Point(184, 14);
			this->groupBox12->Name = L"groupBox12";
			this->groupBox12->Size = System::Drawing::Size(128, 130);
			this->groupBox12->TabIndex = 0;
			this->groupBox12->TabStop = false;
			this->groupBox12->Text = L"Defects mark";
			// 
			// pB_defectS
			// 
			this->pB_defectS->Location = System::Drawing::Point(8, 16);
			this->pB_defectS->Name = L"pB_defectS";
			this->pB_defectS->Size = System::Drawing::Size(110, 110);
			this->pB_defectS->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pB_defectS->TabIndex = 0;
			this->pB_defectS->TabStop = false;
			this->pB_defectS->Click += gcnew System::EventHandler(this, &frm_Defect_inspection::pictureBox4_Click);
			// 
			// groupBox11
			// 
			this->groupBox11->Controls->Add(this->pB_defect);
			this->groupBox11->Location = System::Drawing::Point(30, 14);
			this->groupBox11->Name = L"groupBox11";
			this->groupBox11->Size = System::Drawing::Size(128, 130);
			this->groupBox11->TabIndex = 0;
			this->groupBox11->TabStop = false;
			this->groupBox11->Text = L"Defect";
			// 
			// pB_defect
			// 
			this->pB_defect->Location = System::Drawing::Point(8, 14);
			this->pB_defect->Name = L"pB_defect";
			this->pB_defect->Size = System::Drawing::Size(110, 110);
			this->pB_defect->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pB_defect->TabIndex = 0;
			this->pB_defect->TabStop = false;
			// 
			// groupBox13
			// 
			this->groupBox13->Controls->Add(this->txt_B);
			this->groupBox13->Controls->Add(this->txt_G);
			this->groupBox13->Controls->Add(this->label10);
			this->groupBox13->Controls->Add(this->txt_R);
			this->groupBox13->Controls->Add(this->label7);
			this->groupBox13->Controls->Add(this->txt_Gray);
			this->groupBox13->Controls->Add(this->label8);
			this->groupBox13->Controls->Add(this->label9);
			this->groupBox13->Location = System::Drawing::Point(969, 544);
			this->groupBox13->Name = L"groupBox13";
			this->groupBox13->Size = System::Drawing::Size(101, 117);
			this->groupBox13->TabIndex = 9;
			this->groupBox13->TabStop = false;
			this->groupBox13->Text = L"Def. point";
			// 
			// txt_B
			// 
			this->txt_B->Location = System::Drawing::Point(42, 88);
			this->txt_B->Name = L"txt_B";
			this->txt_B->Size = System::Drawing::Size(46, 20);
			this->txt_B->TabIndex = 1;
			// 
			// txt_G
			// 
			this->txt_G->Location = System::Drawing::Point(42, 65);
			this->txt_G->Name = L"txt_G";
			this->txt_G->Size = System::Drawing::Size(46, 20);
			this->txt_G->TabIndex = 1;
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(12, 91);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(14, 13);
			this->label10->TabIndex = 0;
			this->label10->Text = L"B";
			// 
			// txt_R
			// 
			this->txt_R->Location = System::Drawing::Point(42, 41);
			this->txt_R->Name = L"txt_R";
			this->txt_R->Size = System::Drawing::Size(46, 20);
			this->txt_R->TabIndex = 1;
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(12, 68);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(15, 13);
			this->label7->TabIndex = 0;
			this->label7->Text = L"G";
			// 
			// txt_Gray
			// 
			this->txt_Gray->Location = System::Drawing::Point(42, 18);
			this->txt_Gray->Name = L"txt_Gray";
			this->txt_Gray->Size = System::Drawing::Size(46, 20);
			this->txt_Gray->TabIndex = 1;
			this->txt_Gray->TextChanged += gcnew System::EventHandler(this, &frm_Defect_inspection::textBox9_TextChanged);
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(12, 44);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(15, 13);
			this->label8->TabIndex = 0;
			this->label8->Text = L"R";
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(12, 20);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(29, 13);
			this->label9->TabIndex = 0;
			this->label9->Text = L"Gray";
			// 
			// groupBox14
			// 
			this->groupBox14->Controls->Add(this->txt_BR);
			this->groupBox14->Controls->Add(this->txt_GR);
			this->groupBox14->Controls->Add(this->label11);
			this->groupBox14->Controls->Add(this->txt_RR);
			this->groupBox14->Controls->Add(this->label12);
			this->groupBox14->Controls->Add(this->txt_GrayR);
			this->groupBox14->Controls->Add(this->label13);
			this->groupBox14->Controls->Add(this->label14);
			this->groupBox14->Location = System::Drawing::Point(119, 19);
			this->groupBox14->Name = L"groupBox14";
			this->groupBox14->Size = System::Drawing::Size(113, 117);
			this->groupBox14->TabIndex = 9;
			this->groupBox14->TabStop = false;
			this->groupBox14->Text = L"Ref. point";
			// 
			// txt_BR
			// 
			this->txt_BR->Location = System::Drawing::Point(42, 88);
			this->txt_BR->Name = L"txt_BR";
			this->txt_BR->Size = System::Drawing::Size(61, 20);
			this->txt_BR->TabIndex = 1;
			// 
			// txt_GR
			// 
			this->txt_GR->Location = System::Drawing::Point(42, 65);
			this->txt_GR->Name = L"txt_GR";
			this->txt_GR->Size = System::Drawing::Size(61, 20);
			this->txt_GR->TabIndex = 1;
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Location = System::Drawing::Point(12, 91);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(14, 13);
			this->label11->TabIndex = 0;
			this->label11->Text = L"B";
			// 
			// txt_RR
			// 
			this->txt_RR->Location = System::Drawing::Point(42, 41);
			this->txt_RR->Name = L"txt_RR";
			this->txt_RR->Size = System::Drawing::Size(61, 20);
			this->txt_RR->TabIndex = 1;
			this->txt_RR->TextChanged += gcnew System::EventHandler(this, &frm_Defect_inspection::textBox13_TextChanged);
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Location = System::Drawing::Point(12, 68);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(15, 13);
			this->label12->TabIndex = 0;
			this->label12->Text = L"G";
			// 
			// txt_GrayR
			// 
			this->txt_GrayR->Location = System::Drawing::Point(42, 18);
			this->txt_GrayR->Name = L"txt_GrayR";
			this->txt_GrayR->Size = System::Drawing::Size(61, 20);
			this->txt_GrayR->TabIndex = 1;
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Location = System::Drawing::Point(12, 44);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(15, 13);
			this->label13->TabIndex = 0;
			this->label13->Text = L"R";
			// 
			// label14
			// 
			this->label14->AutoSize = true;
			this->label14->Location = System::Drawing::Point(12, 20);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(29, 13);
			this->label14->TabIndex = 0;
			this->label14->Text = L"Gray";
			// 
			// groupBox15
			// 
			this->groupBox15->Controls->Add(this->btn_clear1);
			this->groupBox15->Controls->Add(this->groupBox16);
			this->groupBox15->Controls->Add(this->groupBox14);
			this->groupBox15->Location = System::Drawing::Point(957, 527);
			this->groupBox15->Name = L"groupBox15";
			this->groupBox15->Size = System::Drawing::Size(344, 145);
			this->groupBox15->TabIndex = 11;
			this->groupBox15->TabStop = false;
			this->groupBox15->Text = L"Defect component values";
			// 
			// btn_clear1
			// 
			this->btn_clear1->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btn_clear1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->btn_clear1->Location = System::Drawing::Point(238, 63);
			this->btn_clear1->Name = L"btn_clear1";
			this->btn_clear1->Size = System::Drawing::Size(100, 69);
			this->btn_clear1->TabIndex = 13;
			this->btn_clear1->Text = L"Result";
			this->btn_clear1->UseVisualStyleBackColor = true;
			// 
			// groupBox16
			// 
			this->groupBox16->Controls->Add(this->txt_Sg);
			this->groupBox16->Location = System::Drawing::Point(238, 19);
			this->groupBox16->Name = L"groupBox16";
			this->groupBox16->Size = System::Drawing::Size(100, 40);
			this->groupBox16->TabIndex = 12;
			this->groupBox16->TabStop = false;
			this->groupBox16->Text = L"Ratio for defect";
			// 
			// txt_Sg
			// 
			this->txt_Sg->Location = System::Drawing::Point(6, 16);
			this->txt_Sg->Name = L"txt_Sg";
			this->txt_Sg->Size = System::Drawing::Size(85, 20);
			this->txt_Sg->TabIndex = 1;
			// 
			// btn_clear
			// 
			this->btn_clear->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btn_clear->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 36, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->btn_clear->Location = System::Drawing::Point(959, 100);
			this->btn_clear->Name = L"btn_clear";
			this->btn_clear->Size = System::Drawing::Size(336, 104);
			this->btn_clear->TabIndex = 12;
			this->btn_clear->Text = L"RESULT";
			this->btn_clear->UseVisualStyleBackColor = true;
			this->btn_clear->Click += gcnew System::EventHandler(this, &frm_Defect_inspection::btn_clear_Click);
			// 
			// rB_ver
			// 
			this->rB_ver->AutoSize = true;
			this->rB_ver->Location = System::Drawing::Point(243, 31);
			this->rB_ver->Name = L"rB_ver";
			this->rB_ver->Size = System::Drawing::Size(60, 17);
			this->rB_ver->TabIndex = 13;
			this->rB_ver->TabStop = true;
			this->rB_ver->Text = L"Vertical";
			this->rB_ver->UseVisualStyleBackColor = true;
			this->rB_ver->CheckedChanged += gcnew System::EventHandler(this, &frm_Defect_inspection::rB_ver_CheckedChanged);
			// 
			// rB_hor
			// 
			this->rB_hor->AutoSize = true;
			this->rB_hor->Location = System::Drawing::Point(243, 54);
			this->rB_hor->Name = L"rB_hor";
			this->rB_hor->Size = System::Drawing::Size(72, 17);
			this->rB_hor->TabIndex = 13;
			this->rB_hor->TabStop = true;
			this->rB_hor->Text = L"Horizontal";
			this->rB_hor->UseVisualStyleBackColor = true;
			this->rB_hor->CheckedChanged += gcnew System::EventHandler(this, &frm_Defect_inspection::rB_hor_CheckedChanged);
			// 
			// groupBox17
			// 
			this->groupBox17->Controls->Add(this->txt_time);
			this->groupBox17->Location = System::Drawing::Point(668, 316);
			this->groupBox17->Name = L"groupBox17";
			this->groupBox17->Size = System::Drawing::Size(275, 64);
			this->groupBox17->TabIndex = 14;
			this->groupBox17->TabStop = false;
			this->groupBox17->Text = L"Processing time";
			// 
			// txt_time
			// 
			this->txt_time->Location = System::Drawing::Point(12, 28);
			this->txt_time->Name = L"txt_time";
			this->txt_time->Size = System::Drawing::Size(253, 20);
			this->txt_time->TabIndex = 0;
			// 
			// frm_Defect_inspection
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1313, 750);
			this->Controls->Add(this->groupBox17);
			this->Controls->Add(this->rB_hor);
			this->Controls->Add(this->rB_ver);
			this->Controls->Add(this->btn_clear);
			this->Controls->Add(this->groupBox10);
			this->Controls->Add(this->groupBox13);
			this->Controls->Add(this->groupBox8);
			this->Controls->Add(this->groupBox7);
			this->Controls->Add(this->groupBox3);
			this->Controls->Add(this->groupBox5);
			this->Controls->Add(this->groupBox2);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->groupBox4);
			this->Controls->Add(this->groupBox15);
			this->Controls->Add(this->groupBox9);
			this->Name = L"frm_Defect_inspection";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Defect Inspection System for Touch Screen Panel: Server-Client Model";
			this->Load += gcnew System::EventHandler(this, &frm_Defect_inspection::frm_Defect_inspection_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_orig))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_roi))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_otsu_hist_hor))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_otsu))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_otsu_hist_ver))->EndInit();
			this->groupBox1->ResumeLayout(false);
			this->groupBox2->ResumeLayout(false);
			this->groupBox3->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_ROIcut))->EndInit();
			this->groupBox4->ResumeLayout(false);
			this->groupBox5->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_en_hor))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_en_ver))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_energy))->EndInit();
			this->groupBox6->ResumeLayout(false);
			this->groupBox6->PerformLayout();
			this->groupBox7->ResumeLayout(false);
			this->groupBox7->PerformLayout();
			this->groupBox8->ResumeLayout(false);
			this->groupBox9->ResumeLayout(false);
			this->groupBox18->ResumeLayout(false);
			this->groupBox18->PerformLayout();
			this->groupBox10->ResumeLayout(false);
			this->groupBox12->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_defectS))->EndInit();
			this->groupBox11->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_defect))->EndInit();
			this->groupBox13->ResumeLayout(false);
			this->groupBox13->PerformLayout();
			this->groupBox14->ResumeLayout(false);
			this->groupBox14->PerformLayout();
			this->groupBox15->ResumeLayout(false);
			this->groupBox16->ResumeLayout(false);
			this->groupBox16->PerformLayout();
			this->groupBox17->ResumeLayout(false);
			this->groupBox17->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void btn_start_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
					
					
					lB_defects->Items->Clear();
					pB_orig->Image = nullptr; pB_orig->Refresh();
					pB_otsu->Image = nullptr; pB_otsu->Refresh();
					pB_otsu_hist_ver->Image = nullptr; pB_otsu_hist_ver->Refresh();
					pB_otsu_hist_hor->Image = nullptr; pB_otsu_hist_hor->Refresh();
					pB_roi->Image = nullptr; pB_roi->Refresh();
					pB_ROIcut->Image = nullptr; pB_ROIcut->Refresh();
					pB_energy->Image = nullptr; pB_energy->Refresh();
					pB_en_hor->Image = nullptr; pB_en_hor->Refresh();
					pB_en_ver->Image = nullptr; pB_en_ver->Refresh();
					pB_defect->Image = nullptr; pB_defect->Refresh();
					pB_defectS->Image = nullptr; pB_defectS->Refresh();
					btn_clear->Text = "RESULT";
					txt_h->Clear(); txt_w->Clear();txt_hl->Clear();txt_time->Clear();
					txt_Gray->Clear();txt_R->Clear();txt_G->Clear();txt_B->Clear();
					txt_GrayR->Clear();txt_RR->Clear();txt_GR->Clear();txt_BR->Clear();txt_Sg->Clear();
					//txt_energythreshold->Clear(); txt_pixellimit->Clear();txt_SD->Clear();
					btn_clear1->Text = "Result";
					// Clear original image Array
					if(R_matrix!=NULL){Matrix2DDelete(R_matrix,InputH);R_matrix=NULL;}
					if(G_matrix!=NULL){Matrix2DDelete(G_matrix,InputH);G_matrix=NULL;}
					if(B_matrix!=NULL){Matrix2DDelete(B_matrix,InputH);B_matrix=NULL;}
					if(Gray_matrix!=NULL){Matrix2DDelete(Gray_matrix,InputH);Gray_matrix=NULL;}
					//Clear otsu Figure Array
					if(otsu_Gray_matrix!=NULL){Matrix2DDelete(otsu_Gray_matrix,InputH);otsu_Gray_matrix=NULL;}
					//Clear ROI figure array
					if(roi_matrix!=NULL){Matrix2DDelete(roi_matrix,RoiH);roi_matrix=NULL;}
					if(roi_R_matrix!=NULL){Matrix2DDelete(roi_R_matrix,RoiH);roi_R_matrix=NULL;}
					if(roi_G_matrix!=NULL){Matrix2DDelete(roi_G_matrix,RoiH);roi_G_matrix=NULL;}
					if(roi_B_matrix!=NULL){Matrix2DDelete(roi_B_matrix,RoiH);roi_B_matrix=NULL;}

					//Clear energy diagram array
					if(energy_matrix!=NULL){Matrix2DDelete(energy_matrix,RoiH);energy_matrix=NULL;}
					//Clear cross-projection array
					if(ExistFieldPosition!=NULL){Matrix2DDelete(ExistFieldPosition,ExistFieldPosition_deleteH);ExistFieldPosition=NULL;}
					
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
						int col_s = img.cols;
						int row_s = img.rows;
						if ((rB_ver->Checked)&&(img.cols > img.rows))
						{
							MessageBox::Show("Please select vertical image type", "Image type not available",
							MessageBoxButtons::OK);
							return;
						}
						if ((rB_hor->Checked)&&(img.cols < img.rows))
						{
							MessageBox::Show("Please select horizontal image type", "Image type not available",
							MessageBoxButtons::OK);
							return;
						}
						
						for (int i = img.cols; i < img.cols+4; i++) {if (col_s%4 != 0) col_s++;}
						for (int i = img.rows; i < img.rows+4; i++) {if (row_s%4 != 0) row_s++;}
						
						// show image
						Mat img_s = Mat(col_s, row_s, img.type());
						resize(img, img_s, img_s.size(), 0, 0, INTER_LINEAR);
						
						
						Mat process_image = img.clone();
						Mat img_show = img.clone();
						
						EnergyThreshold=System::Convert::ToInt32(txt_energythreshold->Text);
						PixelLimit=System::Convert::ToInt32(txt_pixellimit->Text);

						InputH = img.cols;
						InputW = img.rows;

						R_matrix=new int *[InputH];
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
						TIMER_INIT
						TIMER_START
						//=============================The original image done otsu=====================================
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
						if ((cB_all->Checked)||(cB_otsu->Checked)) {imwrite("..\\Results\\otsu.bmp", otsu_image);}
			 
						//------------------------------Display binary image----------------------------
						Mat otsu_image_s = Mat(col_s, row_s, otsu_image.type());
						resize(otsu_image, otsu_image_s, otsu_image_s.size(), 0, 0, INTER_LINEAR);

						
						
						//------------------------------Read the information within otsu_image------------------
						
						otsu_Gray_matrix=new int *[InputH];
						for(int y=0;y<=InputH-1;y++)
						{
							
							otsu_Gray_matrix[y]=new int [InputW];
							for(int x=0;x<=InputW-1;x++)
							{
								otsu_Gray_matrix[y][x]=otsu_image.at<Vec3b>(cv::Point(y, x))[0];
								//otsu_Gray_matrix[y][x] = uchar(double(otsu_image.at<Vec3b>(cv::Point(y, x))[0]+otsu_image.at<Vec3b>(cv::Point(y, x))[1]+otsu_image.at<Vec3b>(cv::Point(y, x))[2])/3.0);
							}
						}



						//=============================The otsu_image do cross projection===========================
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
								if(otsu_Gray_matrix[y][x]>EnergyThreshold)
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
					
						if (rB_ver->Checked) // doc
						{
							//------Otsu_image vertical projection matrix to find the X coordinate of ROI----------------
							for(int i=InputW/2;i>=0;i--)
							{
								if(vertical_value[i]>VerticalMaxValue*0.7)
								{
									roi_image_postion[1][1]=i+roi_cut;
									i=-1;
									//break;
								}
							}
							for(int i=InputW/2;i<=InputW-1;i++)
							{
								if(vertical_value[i]>VerticalMaxValue*0.7)
								{
									roi_image_postion[2][1]=i-roi_cut;
									i=InputW;
									//break;
								}
							}
							//-------------Range of ROI----------------------------------------
							RoiMinH=0;
							RoiMaxH=img.cols;//InputH;
							RoiMinW=roi_image_postion[1][1];
							RoiMaxW=roi_image_postion[2][1];
						}

						/////////////////////////////////////////////////////////////////////////////////////
						if (rB_hor->Checked) // ngang
						{
							//-------------To otsu_image horizontal projection matrix Y coordinates to find the ROI---------
							for(int i=InputH/2;i>=0;i--)
							{
								if(horizon_value[i]>=HorizonMaxValue*0.7)
								{
									roi_image_postion[1][1]=i+roi_cut;
									i=-1;
									//break;
								}
							}
							for(int i=InputH/2;i<=InputH-1;i++)
							{
								if(horizon_value[i]>=HorizonMaxValue*0.7)
								{
									roi_image_postion[2][1]=i-roi_cut;
									i=InputH;
									//break;
								}
							}
							//-------------ROI range----------------------------------------
							RoiMinH=roi_image_postion[1][1];
							RoiMaxH=roi_image_postion[2][1];
							RoiMinW=0;
							RoiMaxW=InputW;//
						}
						//lB_defects->Items->Add(RoiMinH.ToString()+" "+ RoiMinW.ToString()+ " " +  RoiMaxH.ToString() + " " + InputW.ToString());
						//---------------------Draw ROI region--------------------------------------
						line(img_show,cv::Point(RoiMinH,RoiMinW), cv::Point(RoiMinH,RoiMaxW), cv::Scalar( 0, 0, 255 ), 10, 8);
						line(img_show,cv::Point(RoiMinH,RoiMaxW), cv::Point(RoiMaxH,RoiMaxW), cv::Scalar( 0, 0, 255 ), 10, 8);
						line(img_show,cv::Point(RoiMaxH,RoiMaxW), cv::Point(RoiMaxH,RoiMinW), cv::Scalar( 0, 0, 255 ), 10, 8);
						line(img_show,cv::Point(RoiMaxH,RoiMinW), cv::Point(RoiMinH,RoiMinW), cv::Scalar( 0, 0, 255 ), 10, 8);
						if ((cB_all->Checked)||(cB_ROIpaint->Checked)) {imwrite("..\\Results\\ROIpaint.bmp", img_show);}
			
						
						//display ROIpaint image
						Mat img_show_s = Mat(col_s, row_s, img_show.type());
						resize(img_show, img_show_s, img_show_s.size(), 0, 0, INTER_LINEAR);
						
						
						//--------------------Figure depicts the vertical projection otsu_image----------------------
						Mat ver_hist = Mat(InputW, VerticalMaxValue, img.type(),cv::Scalar( 255, 255, 255 ));
						for(int i=0;i<=InputW-1;i++)
						{
							line(ver_hist,cv::Point(0, i), cv::Point(vertical_value[i], i), cv::Scalar( 0, 0, 255 ), 10, 8);
							
						}
						delete[] vertical_value;
						
						int col_ver_hist_s = ver_hist.cols;
						int row_ver_hist_s = ver_hist.rows;
						for (int i = ver_hist.cols; i < ver_hist.cols+4; i++) {if (col_ver_hist_s%4 != 0) col_ver_hist_s++;}
						for (int i = ver_hist.rows; i < ver_hist.rows+4; i++) {if (row_ver_hist_s%4 != 0) row_ver_hist_s++;}
						Mat ver_hist_s = Mat(col_ver_hist_s, row_ver_hist_s, ver_hist.type());
						resize(ver_hist, ver_hist_s, ver_hist_s.size(), 0, 0, INTER_LINEAR);
						

						Mat hor_hist = Mat(HorizonMaxValue,InputH, img.type(),cv::Scalar( 255, 255, 255 ));
						for(int i=0;i<=InputH-1;i++)
						{
							line(hor_hist,cv::Point(i,0), cv::Point(i, horizon_value[i]), cv::Scalar( 0, 0, 255 ), 10, 8);
							
						}
						delete[] horizon_value;

						int col_hor_hist_s = hor_hist.cols;
						int row_hor_hist_s = hor_hist.rows;
						for (int i = hor_hist.cols; i < hor_hist.cols+4; i++) {if (col_hor_hist_s%4 != 0) col_hor_hist_s++;}
						for (int i = hor_hist.rows; i < hor_hist.rows+4; i++) {if (row_hor_hist_s%4 != 0) row_hor_hist_s++;}
						Mat hor_hist_s = Mat(col_hor_hist_s, row_hor_hist_s, hor_hist.type());
						resize(hor_hist, hor_hist_s, hor_hist_s.size(), 0, 0, INTER_LINEAR);
						

						//===============================In Ori figure cut out roi===============================
						RoiH=RoiMaxH-RoiMinH;
						RoiW=RoiMaxW-RoiMinW;
						//if (RoiH % 2 !=0) {RoiH-=1;}
						//if (RoiW % 2 !=0) {RoiW-=1;}
						if (RoiH < 0) {RoiH=0;} else if (RoiH> (InputH-1)) {RoiH = InputH - 1;}
						if (RoiW < 0) {RoiW=0;} else if (RoiW > (InputW - 1)) {RoiW = InputW - 1;}
						Mat roi_image = Mat(RoiW,RoiH, img.type());
						roi_R_matrix=new int *[RoiH];
						roi_G_matrix=new int *[RoiH];
						roi_B_matrix=new int *[RoiH];
						for(int y=0;y<=RoiH-1;y++)
						{
							//Byte *TempRow=(Byte *)Form1->roi_image->ScanLine[y];
							roi_R_matrix[y]=new int [RoiW];
							roi_G_matrix[y]=new int [RoiW];
							roi_B_matrix[y]=new int [RoiW];
							for(int x=0;x<=RoiW-1;x++)
							{
								int Rx=RoiMinW+x;
								int Ry=RoiMinH+y;
								roi_image.at<Vec3b>(cv::Point(y, x))[2]=R_matrix[Ry][Rx];
								roi_image.at<Vec3b>(cv::Point(y, x))[1]=G_matrix[Ry][Rx];
								roi_image.at<Vec3b>(cv::Point(y, x))[0]=B_matrix[Ry][Rx];

								roi_R_matrix[y][x]=roi_image.at<Vec3b>(cv::Point(y, x))[2];
								roi_G_matrix[y][x]=roi_image.at<Vec3b>(cv::Point(y, x))[1];
								roi_B_matrix[y][x]=roi_image.at<Vec3b>(cv::Point(y, x))[0];
							}
						}

						if ((cB_all->Checked)||(cB_ROIcut->Checked)) {imwrite("..\\Results\\ROIcut.bmp", roi_image);}
					
						int col_roi_image_s = roi_image.cols;
						int row_roi_image_s = roi_image.rows;
						for (int i = roi_image.cols; i < roi_image.cols+4; i++) {if (col_roi_image_s%4 != 0) col_roi_image_s++;}
						for (int i = roi_image.rows; i < roi_image.rows+4; i++) {if (row_roi_image_s%4 != 0) row_roi_image_s++;}
						Mat roi_image_s = Mat(col_roi_image_s, row_roi_image_s, roi_image.type());
						resize(roi_image, roi_image_s, roi_image_s.size(), 0, 0, INTER_LINEAR);
						

						Mat energy_image = Mat(RoiW,RoiH, img.type());
						energy_matrix=new int *[RoiH];
						for(int y=0;y<=RoiH-1;y++)
						{
							energy_matrix[y]=new int [RoiW];
						}
						//--------------------Localized energy analysis done for roi_G_matrix--------------------
						ea_process(roi_G_matrix,RoiW,RoiH,false,false,energy_matrix,1); 
						for(int y=0;y<=RoiH-1;y++)
						{
							
							for(int x=0;x<=RoiW-1;x++)
							{
								energy_image.at<Vec3b>(cv::Point(y, x))[2]=energy_matrix[y][x];
								energy_image.at<Vec3b>(cv::Point(y, x))[1]=energy_matrix[y][x];
								energy_image.at<Vec3b>(cv::Point(y, x))[0]=energy_matrix[y][x];
							}
						}
						if ((cB_all->Checked)||(cB_energy->Checked)) {imwrite("..\\Results\\Energy.bmp", energy_image);}
					
						Mat energy_image_s = Mat(col_roi_image_s, row_roi_image_s, energy_image.type());
						resize(energy_image, energy_image_s, energy_image_s.size(), 0, 0, INTER_LINEAR);

						imshow ("", energy_image_s);
						
						//=======================Figure information on energy_matrix do cross projection ========================
						int *ROI_vertical_value=new int [RoiW];
						int *ROI_horizon_value=new int [RoiH];
						// Initial value zero
						for(int i=0;i<=RoiW-1;i++)
						{
							ROI_vertical_value[i]=0;
						}
						for(int i=0;i<=RoiH-1;i++)
						{
							ROI_horizon_value[i]=0;
						}
						for(int y=0;y<=RoiH-1;y++)
						{
							for(int x=0;x<=RoiW-1;x++)
							{
							//-------------energy_matrix The information was greater than the threshold Projection Projection--
								if(energy_matrix[y][x]>EnergyThreshold)
								{
									ROI_vertical_value[x]++;
									ROI_horizon_value[y]++;
								}
							}
						}
						int **ROI_XPosition=new int *[int(RoiW/2)];  // Existence flag (0or1), the beginning of the x-coordinate, x coordinate of the end of the arrangement (skip 0)
						int **ROI_YPosition=new int *[int(RoiH/2)];  // In the presence of the flag (0or1), y coordinates beginning, y coordinates end alignment (skip 0)
						for(int y=0;y<=(RoiW/2)-1;y++)
						{
							ROI_XPosition[y]=new int[3];
							for(int x=0;x<=2;x++)
							ROI_XPosition[y][x]=0;
						}
						for(int y=0;y<=(RoiH/2)-1;y++)
						{
							ROI_YPosition[y]=new int[3];
							for(int x=0;x<=2;x++)
							ROI_YPosition[y][x]=0;
						}
						int ROI_ValueLevel=0;
						int ROI_XPositionCounter=0;
						int ROI_VerticalMaxValue=0;
						for(int i=0;i<=RoiW-1;i++)
						{
							if(ROI_vertical_value[i]>=ROI_VerticalMaxValue)
								ROI_VerticalMaxValue=ROI_vertical_value[i];

							if((ROI_vertical_value[i]>=(3)) && ROI_ValueLevel==0)
							{
								ROI_XPositionCounter++;
								ROI_XPosition[ROI_XPositionCounter][1]=i;
								ROI_ValueLevel=1;
							}
							if((ROI_vertical_value[i]<(3)) && ROI_ValueLevel==1)
							{
								ROI_XPosition[ROI_XPositionCounter][2]=i;
								ROI_XPosition[ROI_XPositionCounter][0]=1;
								ROI_ValueLevel=0;
							}
						}
						ROI_ValueLevel=0;
						int ROI_YPositionCounter=0;
						int ROI_HorizonMaxValue=0;
						for(int i=0;i<=RoiH-1;i++)
						{
							if(ROI_horizon_value[i]>=ROI_HorizonMaxValue)
							ROI_HorizonMaxValue=ROI_horizon_value[i];
							if((ROI_horizon_value[i]>=(3)) && ROI_ValueLevel==0)
							{
								ROI_YPositionCounter++;
								ROI_YPosition[ROI_YPositionCounter][1]=i;
								ROI_ValueLevel=1;
							}
							if((ROI_horizon_value[i]<(3)) && ROI_ValueLevel==1)
							{
								ROI_YPosition[ROI_YPositionCounter][2]=i;
								ROI_YPosition[ROI_YPositionCounter][0]=1;
								ROI_ValueLevel=0;
							}
						}
						//if (ROI_VerticalMaxValue%2 !=0){ROI_VerticalMaxValue-=1;}
						//if (ROI_HorizonMaxValue%2 !=0){ROI_HorizonMaxValue-=1;}

						Mat energy_hist_ver = Mat(RoiW,ROI_VerticalMaxValue, img.type(),cv::Scalar( 255, 255, 255 ));
						for(int i=0;i<=RoiW-1;i++)
						{
							line(energy_hist_ver,cv::Point(0,i), cv::Point(ROI_vertical_value[i], i), cv::Scalar( 0, 0, 255 ), 1, 1);
						}
						delete[] ROI_vertical_value;
						// show image
						int col_energy_hist_ver_s = energy_hist_ver.cols;
						int row_energy_hist_ver_s = energy_hist_ver.rows;
						for (int i = energy_hist_ver.cols; i < energy_hist_ver.cols+4; i++) {if (col_energy_hist_ver_s%4 != 0) col_energy_hist_ver_s++;}
						for (int i = energy_hist_ver.rows; i < energy_hist_ver.rows+4; i++) {if (row_energy_hist_ver_s%4 != 0) row_energy_hist_ver_s++;}
						Mat energy_hist_ver_s = Mat(row_energy_hist_ver_s, col_energy_hist_ver_s, energy_hist_ver.type());
						resize(energy_hist_ver, energy_hist_ver_s, energy_hist_ver_s.size(), 0, 0, INTER_LINEAR);
						
						//-------------------------------Horizontal projection depicting ROI-------------

						Mat energy_hist_hor = Mat(ROI_HorizonMaxValue,RoiH, img.type(),cv::Scalar( 255, 255, 255 ));
						 for(int i=0;i<=RoiH-1;i++)
						{
							
							line(energy_hist_hor,cv::Point(i, 0), cv::Point(i, ROI_horizon_value[i]), cv::Scalar( 0, 0, 255 ), 1, 1);
						}
						delete[] ROI_horizon_value;
						//imshow("",energy_hist_hor);

						int col_energy_hist_hor_s = energy_hist_hor.cols;
						int row_energy_hist_hor_s = energy_hist_hor.rows;
						for (int i = energy_hist_hor.cols; i < energy_hist_hor.cols+4; i++) {if (col_energy_hist_hor_s%4 != 0) col_energy_hist_hor_s++;}
						for (int i = energy_hist_hor.rows; i < energy_hist_hor.rows+4; i++) {if (row_energy_hist_hor_s%4 != 0) row_energy_hist_hor_s++;}
						Mat energy_hist_hor_s = Mat(row_energy_hist_hor_s, col_energy_hist_hor_s, energy_hist_hor.type());
						resize(energy_hist_hor, energy_hist_hor_s, energy_hist_hor_s.size(), 0, 0, INTER_LINEAR);
						
						
						if(ROI_YPositionCounter*ROI_XPositionCounter==0)
						{
							btn_clear->Text = L"GO";
							return;
						}
						else btn_clear->Text = L"NG";

						TIMER_STOP
							txt_time->Text = ((float)elapsedTime).ToString()+" secs";
						
						//----------------------Remove the blank sub-region coordinates------------------------------
						ExistFieldPosition=new int *[(ROI_YPositionCounter*ROI_XPositionCounter+1)];
						ExistFieldPosition_deleteH=(ROI_YPositionCounter*ROI_XPositionCounter+1);
						ExistFieldCounter=0;
						for(int y=0;y<=ROI_YPositionCounter*ROI_XPositionCounter;y++)
						{
							ExistFieldPosition[y]=new int [6];
							for(int x=0;x<=4;x++)
								ExistFieldPosition[y][x]=0;
						}
						for(int y=1;y<=ROI_YPositionCounter;y++)
						{
							for(int x=1;x<=ROI_XPositionCounter;x++)
							{
								int XStart=ROI_XPosition[x][1];
								int XEnd=ROI_XPosition[x][2];
								int YStart=ROI_YPosition[y][1];
								int YEnd=ROI_YPosition[y][2];
								int FieldPixelCounter=0;
								for(int yy=YStart;yy<=YEnd;yy++)
								{
									for(int xx=XStart;xx<=XEnd;xx++)
									{
                                        if(energy_matrix[yy][xx]>=EnergyThreshold)
                                                FieldPixelCounter++;
									}
								}
								//---------------------Defective red marquee size-------------------
								if(FieldPixelCounter>=PixelLimit)
								{
									ExistFieldCounter++;
									ExistFieldPosition[ExistFieldCounter][0]=1;
									ExistFieldPosition[ExistFieldCounter][1]=XStart+(XStart-XEnd);
									ExistFieldPosition[ExistFieldCounter][2]=XEnd-(XStart-XEnd);
									ExistFieldPosition[ExistFieldCounter][3]=YStart-10;
									ExistFieldPosition[ExistFieldCounter][4]=YEnd+10;
									ExistFieldPosition[ExistFieldCounter][5]=FieldPixelCounter;
								}
							}
						}
						//---------------------Clear ROI_XY location information-------------------------------
						Matrix2DDelete(ROI_XPosition,int(RoiW/2)-1);
						Matrix2DDelete(ROI_YPosition,int(RoiH/2)-1);
						//--------------------If the size of each sub-area did not exceed the limit on PASS--------------
						if(ExistFieldCounter==0)
						{
							btn_clear->Text = L"GO";
							return;
							
						}
						else btn_clear->Text = L"NG";
						//-------------------------Show in Listbox--------------------------------
						for(int i=1;i<=ExistFieldCounter;i++)
						{
							lB_defects->Items->Add("No."+i.ToString()+": "+ExistFieldPosition[i][5]+" Pixels");
						}

						//=======================Location map mark flaws in roi_image==============================
       
						for(int i=1;i<=ExistFieldCounter;i++)
						{
							line(roi_image,cv::Point(ExistFieldPosition[i][3], ExistFieldPosition[i][1]), cv::Point(ExistFieldPosition[i][3],ExistFieldPosition[i][2]), cv::Scalar( 0, 0, 255 ), 4, 2);
							line(roi_image,cv::Point(ExistFieldPosition[i][3], ExistFieldPosition[i][2]), cv::Point(ExistFieldPosition[i][4],ExistFieldPosition[i][2]), cv::Scalar( 0, 0, 255 ), 4, 2);
							line(roi_image,cv::Point(ExistFieldPosition[i][4], ExistFieldPosition[i][2]), cv::Point(ExistFieldPosition[i][4],ExistFieldPosition[i][1]), cv::Scalar( 0, 0, 255 ), 4, 2);
							line(roi_image,cv::Point(ExistFieldPosition[i][4], ExistFieldPosition[i][1]), cv::Point(ExistFieldPosition[i][3],ExistFieldPosition[i][1]), cv::Scalar( 0, 0, 255 ), 4, 2);
							//*
							
							int fontFace = FONT_HERSHEY_SCRIPT_SIMPLEX;
							double fontScale = 4;
							int thickness = 4;  
							cv::Point textOrg = cv::Point(ExistFieldPosition[i][3]-20,ExistFieldPosition[i][1]-10) ;
							System::String^ managedString = "test";
							//convert System::String to std::string
							msclr::interop::marshal_context context;
							std::string standardString = context.marshal_as<std::string>(i.ToString());

							cv::putText(roi_image, standardString, textOrg, fontFace, fontScale, Scalar(255,255,0), thickness,4);
							//*/
						}
						if ((cB_all->Checked)||(cB_ROIcutsign->Checked)) {imwrite("..\\Results\\ROIcutsign.bmp", roi_image);}
					
						//Display images
						Mat roi_image_s1 = Mat(row_roi_image_s, col_roi_image_s, roi_image.type());
						resize(roi_image, roi_image_s1, roi_image_s1.size(), 0, 0, INTER_LINEAR);
						pB_orig->Image  = gcnew    
						System::Drawing::Bitmap(img_s.size().width, img_s.size().height, img_s.step,
						System::Drawing::Imaging::PixelFormat::Format24bppRgb,(System::IntPtr)img_s.data);
						pB_orig->Refresh();
						pB_otsu->Image  = gcnew    
						System::Drawing::Bitmap(otsu_image_s.size().width, otsu_image_s.size().height,otsu_image_s.step,
						System::Drawing::Imaging::PixelFormat::Format24bppRgb,(System::IntPtr) otsu_image_s.data);
						pB_otsu->Refresh();
						pB_roi->Image  = gcnew    
						System::Drawing::Bitmap(img_show_s.size().width, img_show_s.size().height,img_show_s.step,
						System::Drawing::Imaging::PixelFormat::Format24bppRgb,(System::IntPtr) img_show_s.data);
						pB_roi->Refresh();
						pB_otsu_hist_ver->Image  = gcnew    
						System::Drawing::Bitmap(ver_hist_s.size().width, ver_hist_s.size().height,ver_hist_s.step,
						System::Drawing::Imaging::PixelFormat::Format24bppRgb,(System::IntPtr) ver_hist_s.data);
						pB_otsu_hist_ver->Refresh();
						pB_otsu_hist_hor->Image  = gcnew    
						System::Drawing::Bitmap(hor_hist_s.size().width, hor_hist_s.size().height,hor_hist_s.step,
						System::Drawing::Imaging::PixelFormat::Format24bppRgb,(System::IntPtr) hor_hist_s.data);
						pB_otsu_hist_hor->Refresh();
						pB_ROIcut->Image  = gcnew    
						System::Drawing::Bitmap(roi_image_s.size().width, roi_image_s.size().height,roi_image_s.step,
						System::Drawing::Imaging::PixelFormat::Format24bppRgb,(System::IntPtr) roi_image_s.data);
						pB_ROIcut->Refresh();
						pB_energy->Image  = gcnew    
						System::Drawing::Bitmap(energy_image_s.size().width, energy_image_s.size().height,energy_image_s.step,
						System::Drawing::Imaging::PixelFormat::Format24bppRgb,(System::IntPtr) energy_image_s.data);
						pB_energy->Refresh();
						pB_en_ver->Image  = gcnew    
						System::Drawing::Bitmap(energy_hist_ver_s.size().width, energy_hist_ver_s.size().height,energy_hist_ver_s.step,
						System::Drawing::Imaging::PixelFormat::Format24bppRgb,(System::IntPtr) energy_hist_ver_s.data);
						pB_en_ver->Refresh();
						pB_en_hor->Image  = gcnew    
						System::Drawing::Bitmap(energy_hist_hor_s.size().width, energy_hist_hor_s.size().height,energy_hist_hor_s.step,
						System::Drawing::Imaging::PixelFormat::Format24bppRgb,(System::IntPtr) energy_hist_hor_s.data);
						pB_en_hor->Refresh();
						pB_ROIcut->Image  = gcnew    
						System::Drawing::Bitmap(roi_image_s1.size().width, roi_image_s1.size().height,roi_image_s1.step,
						System::Drawing::Imaging::PixelFormat::Format24bppRgb,(System::IntPtr) roi_image_s1.data);
						pB_ROIcut->Refresh();


						step++;
					}
					
			 }
//=========================================================================================================================================

	private: System::Void frm_Defect_inspection_Load(System::Object^  sender, System::EventArgs^  e) 
			 {
				 txt_energythreshold->Text = "128";
				 txt_pixellimit->Text = "50";
				 txt_SD->Text = "0.5";
				 rB_ver->Checked = true;
				 //pB_point->Visible = false;
			 }
private: System::Void B_otsu_hist_ver_Click(System::Object^  sender, System::EventArgs^  e)
		 {
		
		 }
private: System::Void textBox2_TextChanged(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void btn_clear_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			// Clear original image Array
			if(R_matrix!=NULL){Matrix2DDelete(R_matrix,InputH);R_matrix=NULL;}
			if(G_matrix!=NULL){Matrix2DDelete(G_matrix,InputH);G_matrix=NULL;}
			if(B_matrix!=NULL){Matrix2DDelete(B_matrix,InputH);B_matrix=NULL;}
			if(Gray_matrix!=NULL){Matrix2DDelete(Gray_matrix,InputH);Gray_matrix=NULL;}
			//Clear otsu Figure Array
			if(otsu_Gray_matrix!=NULL){Matrix2DDelete(otsu_Gray_matrix,InputH);otsu_Gray_matrix=NULL;}
			//Clear ROI figure array
			if(roi_matrix!=NULL){Matrix2DDelete(roi_matrix,RoiH);roi_matrix=NULL;}
			if(roi_R_matrix!=NULL){Matrix2DDelete(roi_R_matrix,RoiH);roi_R_matrix=NULL;}
			if(roi_G_matrix!=NULL){Matrix2DDelete(roi_G_matrix,RoiH);roi_G_matrix=NULL;}
			if(roi_B_matrix!=NULL){Matrix2DDelete(roi_B_matrix,RoiH);roi_B_matrix=NULL;}

			//Clear energy diagram array
			if(energy_matrix!=NULL){Matrix2DDelete(energy_matrix,RoiH);energy_matrix=NULL;}
			//Clear cross-projection array
			if(ExistFieldPosition!=NULL){Matrix2DDelete(ExistFieldPosition,ExistFieldPosition_deleteH);ExistFieldPosition=NULL;}
			lB_defects->Items->Clear();
			pB_orig->Image = nullptr; pB_orig->Refresh();
			pB_otsu->Image = nullptr; pB_otsu->Refresh();
			pB_otsu_hist_ver->Image = nullptr; pB_otsu_hist_ver->Refresh();
			pB_otsu_hist_hor->Image = nullptr; pB_otsu_hist_hor->Refresh();
			pB_roi->Image = nullptr; pB_roi->Refresh();
			pB_ROIcut->Image = nullptr; pB_ROIcut->Refresh();
			pB_energy->Image = nullptr; pB_energy->Refresh();
			pB_en_hor->Image = nullptr; pB_en_hor->Refresh();
			pB_en_ver->Image = nullptr; pB_en_ver->Refresh();
			pB_defect->Image = nullptr; pB_defect->Refresh();
			pB_defectS->Image = nullptr; pB_defectS->Refresh();
			btn_clear->Text = "RESULT";
			txt_h->Clear(); txt_w->Clear();txt_hl->Clear();txt_time->Clear();
			txt_Gray->Clear();txt_R->Clear();txt_G->Clear();txt_B->Clear();
			txt_GrayR->Clear();txt_RR->Clear();txt_GR->Clear();txt_BR->Clear();txt_Sg->Clear();
			//txt_energythreshold->Clear(); txt_pixellimit->Clear();txt_SD->Clear();
			btn_clear1->Text = "Result";
			
		}
private: System::Void rB_ver_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			 // Clear original image Array
			if(R_matrix!=NULL){Matrix2DDelete(R_matrix,InputH);R_matrix=NULL;}
			if(G_matrix!=NULL){Matrix2DDelete(G_matrix,InputH);G_matrix=NULL;}
			if(B_matrix!=NULL){Matrix2DDelete(B_matrix,InputH);B_matrix=NULL;}
			if(Gray_matrix!=NULL){Matrix2DDelete(Gray_matrix,InputH);Gray_matrix=NULL;}
			//Clear otsu Figure Array
			if(otsu_Gray_matrix!=NULL){Matrix2DDelete(otsu_Gray_matrix,InputH);otsu_Gray_matrix=NULL;}
			//Clear ROI figure array
			if(roi_matrix!=NULL){Matrix2DDelete(roi_matrix,RoiH);roi_matrix=NULL;}
			if(roi_R_matrix!=NULL){Matrix2DDelete(roi_R_matrix,RoiH);roi_R_matrix=NULL;}
			if(roi_G_matrix!=NULL){Matrix2DDelete(roi_G_matrix,RoiH);roi_G_matrix=NULL;}
			if(roi_B_matrix!=NULL){Matrix2DDelete(roi_B_matrix,RoiH);roi_B_matrix=NULL;}

			//Clear energy diagram array
			if(energy_matrix!=NULL){Matrix2DDelete(energy_matrix,RoiH);energy_matrix=NULL;}
			//Clear cross-projection array
			if(ExistFieldPosition!=NULL){Matrix2DDelete(ExistFieldPosition,ExistFieldPosition_deleteH);ExistFieldPosition=NULL;}
			if (rB_ver->Checked == true) {rB_hor->Checked = false;}
			 lB_defects->Items->Clear();
			 pB_orig->Image = nullptr; pB_orig->Refresh();
			 pB_otsu->Image = nullptr; pB_otsu->Refresh();
			 pB_otsu_hist_ver->Image = nullptr; pB_otsu_hist_ver->Refresh();
			 pB_otsu_hist_hor->Image = nullptr; pB_otsu_hist_hor->Refresh();
			 pB_roi->Image = nullptr; pB_roi->Refresh();
			 pB_ROIcut->Image = nullptr; pB_ROIcut->Refresh();
			 pB_energy->Image = nullptr; pB_energy->Refresh();
			 pB_en_hor->Image = nullptr; pB_en_hor->Refresh();
			 pB_en_ver->Image = nullptr; pB_en_ver->Refresh();
			 pB_defect->Image = nullptr; pB_defect->Refresh();
			 pB_defectS->Image = nullptr; pB_defectS->Refresh();
			 btn_clear->Text = "RESULT";
			 txt_h->Clear(); txt_w->Clear();txt_hl->Clear();txt_time->Clear();
			 txt_Gray->Clear();txt_R->Clear();txt_G->Clear();txt_B->Clear();
			 txt_GrayR->Clear();txt_RR->Clear();txt_GR->Clear();txt_BR->Clear();txt_Sg->Clear();
			 //txt_energythreshold->Clear(); txt_pixellimit->Clear();txt_SD->Clear();
			 btn_clear1->Text = "Result";
			 
		 }
private: System::Void rB_hor_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			 // Clear original image Array
			 if(R_matrix!=NULL){Matrix2DDelete(R_matrix,InputH);R_matrix=NULL;}
			 if(G_matrix!=NULL){Matrix2DDelete(G_matrix,InputH);G_matrix=NULL;}
			 if(B_matrix!=NULL){Matrix2DDelete(B_matrix,InputH);B_matrix=NULL;}
			 if(Gray_matrix!=NULL){Matrix2DDelete(Gray_matrix,InputH);Gray_matrix=NULL;}
			 //Clear otsu Figure Array
			 if(otsu_Gray_matrix!=NULL){Matrix2DDelete(otsu_Gray_matrix,InputH);otsu_Gray_matrix=NULL;}
			 //Clear ROI figure array
			 if(roi_matrix!=NULL){Matrix2DDelete(roi_matrix,RoiH);roi_matrix=NULL;}
			 if(roi_R_matrix!=NULL){Matrix2DDelete(roi_R_matrix,RoiH);roi_R_matrix=NULL;}
			 if(roi_G_matrix!=NULL){Matrix2DDelete(roi_G_matrix,RoiH);roi_G_matrix=NULL;}
			 if(roi_B_matrix!=NULL){Matrix2DDelete(roi_B_matrix,RoiH);roi_B_matrix=NULL;}

			 //Clear energy diagram array
			 if(energy_matrix!=NULL){Matrix2DDelete(energy_matrix,RoiH);energy_matrix=NULL;}
			 //Clear cross-projection array
			 if(ExistFieldPosition!=NULL){Matrix2DDelete(ExistFieldPosition,ExistFieldPosition_deleteH);ExistFieldPosition=NULL;}
			 if (rB_ver->Checked == false){ rB_hor->Checked = true;}
			 lB_defects->Items->Clear();
			 pB_orig->Image = nullptr; pB_orig->Refresh();
			 pB_otsu->Image = nullptr; pB_otsu->Refresh();
			 pB_otsu_hist_ver->Image = nullptr; pB_otsu_hist_ver->Refresh();
			 pB_otsu_hist_hor->Image = nullptr; pB_otsu_hist_hor->Refresh();
			 pB_roi->Image = nullptr; pB_roi->Refresh();
			 pB_ROIcut->Image = nullptr; pB_ROIcut->Refresh();
			 pB_energy->Image = nullptr; pB_energy->Refresh();
			 pB_en_hor->Image = nullptr; pB_en_hor->Refresh();
			 pB_en_ver->Image = nullptr; pB_en_ver->Refresh();
			 pB_defect->Image = nullptr; pB_defect->Refresh();
			 pB_defectS->Image = nullptr; pB_defectS->Refresh();
			 btn_clear->Text = "RESULT";
			 txt_h->Clear(); txt_w->Clear();txt_hl->Clear();txt_time->Clear();
			 txt_Gray->Clear();txt_R->Clear();txt_G->Clear();txt_B->Clear();
			 txt_GrayR->Clear();txt_RR->Clear();txt_GR->Clear();txt_BR->Clear();txt_Sg->Clear();
			 //txt_energythreshold->Clear(); txt_pixellimit->Clear();txt_SD->Clear();
			 btn_clear1->Text = "Result";
		 }
private: System::Void lB_defects_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			 //---------------------Declared defective component analysis-----------------------------------------
			Mat *FieldR,*FieldG,*FieldB;
			int Defect_Gray_max=0,Defect_Gray_max_x_position=0,Defect_Gray_max_y_position=0;
			int Defect_Gray_max_R=0,Defect_Gray_max_G=0,Defect_Gray_max_B=0;
			int Defect_Gray_min=255;
			int Defect_Gray_min_R=0,Defect_Gray_min_G=0,Defect_Gray_min_B=0;
			int Defect_Delta_R=0,Defect_Delta_G=0,Defect_Delta_B=0,Defect_Delta_Avg=0;
			double IndexStandard=0,Defect_Sg=0,Defect_Sg_maxvalue=0,Defect_result=0;
			//=======================Cross-projection-related settings=======================================
			int NowIndex=lB_defects->SelectedIndex+1;
			int NowH=ExistFieldPosition[NowIndex][4]-ExistFieldPosition[NowIndex][3];
			int NowW=ExistFieldPosition[NowIndex][2]-ExistFieldPosition[NowIndex][1];
			
			
			//-------------------------Sub-regional detail information display---------------------------
			txt_h->Text=NowH.ToString()+" Pixels ("+(float(NowH)*25).ToString()+" um)";   //According to Master On: 25um / pixel
			txt_w->Text=(NowW).ToString()+" Pixels ("+(float(NowW)*25).ToString()+" um)";
			txt_hl->Text=(ExistFieldPosition[NowIndex][5]).ToString()+" Pixels ";

			Mat process_image2 = Mat(NowW, NowH, CV_8UC3);//,cv::Scalar( 255, 255, 255 ));
			FieldR=new Mat(NowW, NowH, CV_8UC3);
			FieldG=new Mat(NowW, NowH, CV_8UC3);
			FieldB=new Mat(NowW, NowH, CV_8UC3);
			
			//------------------Figure out the flaws retrieve information from roi_image-------------------------
			for(int y=0;y<=NowH-1;y++)
			{
                for(int x=0;x<=NowW-1;x++)
                {
                        int Nx=ExistFieldPosition[NowIndex][1]+x;
                        int Ny=ExistFieldPosition[NowIndex][3]+y;

						if(Nx<=0) {Nx=0;}
						else if (Nx>=RoiW-1) {Nx=RoiW-1;}

						if(Ny<=0) {Ny=0;}
                        else if (Ny>=RoiH-1)
                                Ny=RoiH-1;

                        process_image2.at<Vec3b>(cv::Point(y, x))[2]=roi_R_matrix[Ny][Nx];
                        process_image2.at<Vec3b>(cv::Point(y, x))[1]=roi_G_matrix[Ny][Nx];
                        process_image2.at<Vec3b>(cv::Point(y, x))[0]=roi_B_matrix[Ny][Nx];
                }
			}

			


			//--------------------Show defect map--------------------------------------
			int NowW_s = NowW;
			int NowH_s = NowH;
			for (int i = NowW; i < NowW+4; i++)
				if (NowW_s%4 != 0) {NowW_s++;}
			for (int i = NowH; i < NowH+4; i++)
				if (NowH_s%4 != 0) {NowH_s++;}
			//Mat process_image2_s;
			Mat process_image2_s = Mat(NowW_s, NowH_s, CV_8UC3);
			resize(process_image2, process_image2_s, process_image2_s.size(), 0, 0, INTER_LINEAR);
			pB_defect->Image  = gcnew    
			System::Drawing::Bitmap(process_image2_s.size().width, process_image2_s.size().height,process_image2_s.step,
			System::Drawing::Imaging::PixelFormat::Format24bppRgb,(System::IntPtr) process_image2_s.data);
			pB_defect->Refresh();
			//========================Defective component analysis==========================================
			//--------------------Declared canvas size--------------------------------------
			Defect_InputH=NowH;//process_image2.rows;
			Defect_InputW=NowW;//process_image2.cols;
			//--------------------Declared access matrix size----------------------------------
			Defect_R_matrix=new int *[Defect_InputH];
			Defect_G_matrix=new int *[Defect_InputH];
			Defect_B_matrix=new int *[Defect_InputH];
			Defect_Gray_matrix=new int *[Defect_InputH];
			//--------------------Information interpret blueprints loop start----------------------------------
			for(int y=0;y<=Defect_InputH-1;y++)
			{
                //Byte *pixel_row=(Byte *)Form1->process_image2->ScanLine[y];
                Defect_R_matrix[y]=new int [Defect_InputW];
                Defect_G_matrix[y]=new int [Defect_InputW];
                Defect_B_matrix[y]=new int [Defect_InputW];
                Defect_Gray_matrix[y]=new int [Defect_InputW];
                for(int x=0;x<=Defect_InputW-1;x++)
                {
                        Defect_R_matrix[y][x]=process_image2.at<Vec3b>(cv::Point(y, x))[2];
                        Defect_G_matrix[y][x]=process_image2.at<Vec3b>(cv::Point(y, x))[1];
                        Defect_B_matrix[y][x]=process_image2.at<Vec3b>(cv::Point(y, x))[0];
                        Defect_Gray_matrix[y][x]=int(double(process_image2.at<Vec3b>(cv::Point(y, x))[2]+process_image2.at<Vec3b>(cv::Point(y, x))[1]+process_image2.at<Vec3b>(cv::Point(y, x))[0])/3.0);
                        //-------Find gray brightest point (flaws) are light, the contrast backlight------
                        if(Defect_Gray_max<=Defect_Gray_matrix[y][x])
                        {
                                Defect_Gray_max=Defect_Gray_matrix[Defect_Gray_max_y_position][Defect_Gray_max_x_position];
                                Defect_Gray_max_R=Defect_R_matrix[Defect_Gray_max_y_position][Defect_Gray_max_x_position];
                                Defect_Gray_max_G=Defect_G_matrix[Defect_Gray_max_y_position][Defect_Gray_max_x_position];
                                Defect_Gray_max_B=Defect_B_matrix[Defect_Gray_max_y_position][Defect_Gray_max_x_position];
                                //-------Record XY coordinate axes----------------------------
                                Defect_Gray_max_x_position=x;
                                Defect_Gray_max_y_position=y; //©T©w
                        }
                }
			}
			//--------------Fixed Y-axis coordinate, retrieve profile information--------------------------------
			for(int y=Defect_Gray_max_y_position;y<=Defect_Gray_max_y_position;y++)
			{
                for(int x=0;x<=Defect_InputW-1;x++)
                {
                        //Are looking for light gray under the darkest point (non-defective reference point), contrary backlight -
                        if(Defect_Gray_min>=Defect_Gray_matrix[y][x])
                        {
                                Defect_Gray_min=Defect_Gray_matrix[y][x];
                                Defect_Gray_min_R=Defect_R_matrix[y][x];
                                Defect_Gray_min_G=Defect_G_matrix[y][x];
                                Defect_Gray_min_B=Defect_B_matrix[y][x];
                        }
                }
			}
			//---------------The amount of variation was calculated for each cross-sectional view of the three-channel rgb----------------------
			Defect_Delta_R=Defect_Gray_max_R-Defect_Gray_min_R;
			Defect_Delta_G=Defect_Gray_max_G-Defect_Gray_min_G;
			Defect_Delta_B=Defect_Gray_max_B-Defect_Gray_min_B;
			Defect_Delta_Avg=int((Defect_Delta_R+Defect_Delta_G+Defect_Delta_B)/3.0);
			//---------------Calculate the rate of change in g channels------------------------------------
			Defect_Sg=((Defect_Delta_G-Defect_Delta_Avg)*100/Defect_Delta_Avg);
			txt_Sg->Text=(Defect_Sg).ToString()+"%";
			//===========================Classification results===========================================
			//---------------Classification thresholds-----------------------------------------------
			IndexStandard = System::Convert::ToDouble(txt_SD->Text);//IndexStandard=StrToFloat(Form1->Edit6->Text); 
			//---------------Rate of change over the limit ----------------------------------------
			Defect_Sg_maxvalue=ClassificationIndexScale+IndexStandard;
			//---------------Calculation of the classification results-------------------------------------------
			//txt_GrayR->Text=(Defect_result).ToString()+"%";//chu y
			Defect_result=(Defect_Sg/(Defect_Sg_maxvalue*100))*100;
			
			if(Defect_result>=100)
                Defect_result=99;
			else if(Defect_result<=0)
                Defect_result=1;

			if(Defect_result>=50)
                btn_clear1->Text=(Defect_result).ToString()+"% for scratches";
			else if(Defect_result<50)
                btn_clear1->Text=(100-Defect_result).ToString()+"% for dust";
			//======================On roi_image map marker location=================================
			if (rB_hor->Checked)
			{
				for(int i=1;i<=ExistFieldCounter;i++)
				{
				
				line(process_image2,cv::Point(Defect_Gray_max_y_position+1, 0), 
					           cv::Point(Defect_Gray_max_y_position+1, Defect_InputW-1), cv::Scalar( 0, 0, 255 ), 0.5, 0.5);
				line(process_image2,cv::Point(Defect_Gray_max_y_position+1, Defect_InputW-1), 
					           cv::Point(Defect_Gray_max_y_position-1, Defect_InputW-1), cv::Scalar( 0, 0, 255 ), 0.5, 0.5);
				line(process_image2,cv::Point(Defect_Gray_max_y_position-1, Defect_InputW-1), 
					           cv::Point(Defect_Gray_max_y_position-1, 0), cv::Scalar( 0, 0, 255 ), 0.5, 0.5);
				line(process_image2,cv::Point(Defect_Gray_max_y_position-1, 0), 
					           cv::Point(Defect_Gray_max_y_position+1, 0), cv::Scalar( 0, 0, 255 ), 0.5, 0.5);
				
							   
				}
			}
			if (rB_ver->Checked)
			{
				for(int i=1;i<=ExistFieldCounter;i++)
				{
				
				line(process_image2,cv::Point(0,Defect_Gray_max_x_position+1), 
					           cv::Point(Defect_InputH-1, Defect_Gray_max_x_position+1), cv::Scalar( 0, 0, 255 ), 0.5, 0.5);
				line(process_image2,cv::Point(Defect_InputH-1, Defect_Gray_max_x_position+1), 
					           cv::Point(Defect_InputH-1, Defect_Gray_max_x_position-1), cv::Scalar( 0, 0, 255 ), 0.5, 0.5);
				line(process_image2,cv::Point(Defect_InputH-1, Defect_Gray_max_x_position-1), 
					           cv::Point(0, Defect_Gray_max_x_position-1), cv::Scalar( 0, 0, 255 ), 0.5, 0.5);
				line(process_image2,cv::Point(0, Defect_Gray_max_x_position-1), 
					           cv::Point(0, Defect_Gray_max_x_position+1), cv::Scalar( 0, 0, 255 ), 0.5, 0.5);
							   
				}
			}
			Mat process_image2_s1 = Mat(NowW_s, NowH_s, CV_8UC3);
			resize(process_image2, process_image2_s1, process_image2_s.size(), 0, 0, INTER_LINEAR);
			pB_defectS->Image  = gcnew    
			System::Drawing::Bitmap(process_image2_s1.size().width, process_image2_s1.size().height,process_image2_s1.step,
			System::Drawing::Imaging::PixelFormat::Format24bppRgb,(System::IntPtr) process_image2_s1.data);
			pB_defectS->Refresh();

			txt_Sg->Text=(Defect_Sg).ToString()+"%";
			txt_Gray->Text=Defect_Gray_max.ToString();
			txt_R->Text=Defect_Gray_max_R.ToString();
			txt_G->Text=Defect_Gray_max_G.ToString();
			txt_B->Text=Defect_Gray_max_B.ToString();
			txt_GrayR->Text=Defect_Gray_min.ToString();
			txt_RR->Text=Defect_Gray_min_R.ToString();
			txt_GR->Text=Defect_Gray_min_G.ToString();
			txt_BR->Text=Defect_Gray_min_B.ToString();
			





			
		 }
private: System::Void pictureBox4_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void textBox13_TextChanged(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void textBox9_TextChanged(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void pB_otsu_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void txt_h_TextChanged(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void txt_SD_TextChanged(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void label4_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void label5_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void txt_pixellimit_TextChanged(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void label6_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void txt_energythreshold_TextChanged(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void groupBox9_Enter(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void groupBox6_Enter(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void groupBox8_Enter(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void pictureBox1_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void groupBox4_Enter(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void pB_orig_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void pB_ROIcut_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void cB_all_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if (cB_all->Checked)
			 {
				 cB_otsu->Checked = false;
				 cB_ROIcut->Checked = false;
				 cB_ROIpaint->Checked = false;
				 cB_energy->Checked = false;
				 cB_ROIcutsign->Checked = false;
			 }
		 }
private: System::Void pB_energy_Click(System::Object^  sender, System::EventArgs^  e) {
		 }

};
}
