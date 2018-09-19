#pragma once

#include <msclr\marshal_cppstd.h>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

using namespace System::Runtime::InteropServices;
using namespace cv;
using namespace std;
using namespace System::Data;
using namespace System::Drawing;
using namespace msclr::interop;


static Mat gradientface(Mat OriImg)
{
	
	int OriH,OriW;
	OriH=OriImg.rows;
	OriW=OriImg.cols;

	Mat OriImg_d = Mat(OriH, OriW, CV_64F);
	OriImg.convertTo(OriImg_d, CV_64F);

	double** OriR = (double**)malloc( OriH*sizeof(double* )+OriW*OriH*sizeof(double));
	double* OriR_j = (double* )(OriR + OriH);
	for(int y = 0; y!=OriH ; ++y, OriR_j+=OriW )
	OriR[y] = OriR_j;

	for(int y=0;y<OriH;y++)
	{
                        
        for(int x=0;x<OriW;x++)
        {
                OriR[y][x]= (double)OriImg_d.at<double>(cv::Point(x, y));
        }
	}

	double min_orir=999999;
	double max_orir=0;
	for(int y=0;y<OriH;y++)
	{
        for(int x=0;x<OriW;x++)
        {
			if(OriR[y][x]<min_orir) min_orir=OriR[y][x];
            if(OriR[y][x]>max_orir) max_orir=OriR[y][x];
        }
	}
	//lB_result->Items->Add("min values: " + min_orir.ToString());
	//lB_result->Items->Add("max values: " + max_orir.ToString());
					
	double mean_r=0;
	double sigma_r=0;
	for(int y=0;y<OriH;y++)
	{
        for(int x=0;x<OriW;x++)
        {
                mean_r=mean_r+OriR[y][x];
        }
	}
	mean_r=mean_r/(OriH*OriW);

	for(int y=0;y<OriH;y++)
	{
        for(int x=0;x<OriW;x++)
        {
                sigma_r=sigma_r+((OriR[y][x]-mean_r)*(OriR[y][x]-mean_r));
        }
	}
	sigma_r=sqrt(sigma_r/(OriH*OriW));

	double** GauR = (double**)malloc( OriH*sizeof(double* )+OriW*OriH*sizeof(double));
	double* GauR_j = (double* )(GauR + OriH);
	for(int y = 0; y!=OriH ; ++y, GauR_j+=OriW )
	GauR[y] = GauR_j;
	double pi=3.14159;
	for(int y=0;y<OriH;y++)
	{
        for(int x=0;x<OriW;x++)
        {
                GauR[y][x]=1/(2*pi*sigma_r*sigma_r)*pow(2.71828,((-1*(OriR[y][x])-mean_r)*(OriR[y][x]-mean_r))/(2*sigma_r*sigma_r));
        }
	}

	// diplay gaussian image
	double max_Gaussf=(-999999);
	double min_Gaussf=999999;
	for(int y=0;y<OriH;y++)
	{
        for(int x=0;x<OriW;x++)
        {
                if(GauR[y][x]>max_Gaussf) max_Gaussf=GauR[y][x];
                if(GauR[y][x]<min_Gaussf) min_Gaussf=GauR[y][x];
        }
	}
	Mat GaussfImg = Mat(OriH, OriW, CV_8UC1);
	for(int y=0;y<OriH;y++)
	{
        for(int x=0;x<OriW;x++)
        {
                GauR[y][x]=(GauR[y][x]-min_Gaussf)/(max_Gaussf-min_Gaussf)*255;
                GaussfImg.at<unsigned char>(cv::Point(x, y)) = (unsigned char)GauR[y][x];
        }
	}
	//imshow("Gauss", GaussfImg);

	///
	//------------Gradientfaces---------------
	double** graR = (double**)malloc( OriH*sizeof(double* )+OriW*OriH*sizeof(double));
	double* graR_j = (double* )(graR + OriH);
	for(int y = 0; y!=OriH ; ++y, graR_j+=OriW )
	graR[y] = graR_j;

	double gau_r_x;
	double gau_r_y;

	for(int y=0;y<OriH;y++)
	{
        for(int x=0;x<OriW;x++)
        {
                gau_r_x=0;
                gau_r_y=0;

                if(x!=0)
                {
                        if(x!=OriW-1)
                        {
                                gau_r_x=((GauR[y][x-1]-GauR[y][x])+(GauR[y][x]-GauR[y][x+1]))/2;
                        }
                        else
                        {
                                gau_r_x=((GauR[y][x-1]-GauR[y][x])+(GauR[y][x]-0))/2;
                        }
                }
                else
                {
                        gau_r_x=((0-GauR[y][x])+(GauR[y][x]-GauR[y][x+1]))/2;
                }
                if(y!=0)
                {
                        if(y!=OriH-1)
                        {
                                gau_r_y=((GauR[y-1][x]-GauR[y][x])+(GauR[y][x]-GauR[y+1][x]))/2;
                        }
                        else
                        {
                                gau_r_y=((GauR[y-1][x]-GauR[y][x])+(GauR[y][x]-0))/2;
                        }
                }
                else
                {
                        gau_r_y=((0-GauR[y][x])+(GauR[y][x]-GauR[y+1][x]))/2;
                }

                if(gau_r_x*gau_r_y>0)
                {
                        if(gau_r_x>0)
                        {
                                graR[y][x]=atan(gau_r_y/gau_r_x);
                        }
                        else
                        {
                                graR[y][x]=pi+atan(gau_r_y/gau_r_x);
                        }
                }
                if(gau_r_x*gau_r_y<0)
                {
                        if(gau_r_x>0)
                        {
                                graR[y][x]=(2*pi)+atan(gau_r_y/gau_r_x);
                        }
                        else
                        {
                                graR[y][x]=pi+atan(gau_r_y/gau_r_x);
                        }
                }
                if(gau_r_x*gau_r_y==0)
                {
                        if(gau_r_x==0)
                        {
                                if(gau_r_y>=0)
                                {
                                        graR[y][x]=pi/2;
                                }
                                else
                                {
                                        graR[y][x]=(3*pi)/2;
                                }
                        }
                        else
                        {
                                if(gau_r_x>0)
                                {
                                        graR[y][x]=0;
                                }
                                else
                                {
                                        graR[y][x]=pi;
                                }
                        }
                }
                graR[y][x]=(graR[y][x]*180)/pi;
			}
	}

	double max_r=(-9999);
	double min_r=9999;
	for(int y=0;y<OriH;y++)
	{
        for(int x=0;x<OriW;x++)
        {
                if(graR[y][x]>max_r) max_r=graR[y][x];
                if(graR[y][x]<min_r) min_r=graR[y][x];
        }
	}
	Mat graRImg = Mat(OriH, OriW, CV_8UC1);
	for(int y=0;y<OriH;y++)
	{
        for(int x=0;x<OriW;x++)
        {
                graR[y][x]=(graR[y][x]-min_r)/(max_r-min_r)*255;
                graRImg.at<unsigned char>(cv::Point(x, y)) = (unsigned char)graR[y][x];
        }
	}
	return graRImg;
}
////////////

namespace Gradientfaces {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for frm_gradientfaces
	/// </summary>
	public ref class frm_gradientfaces : public System::Windows::Forms::Form
	{
	public:
		frm_gradientfaces(void)
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
		~frm_gradientfaces()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  label28;
	protected: 
	private: System::Windows::Forms::TextBox^  txt_savefolder;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::ComboBox^  cB_dataname;
	private: System::Windows::Forms::PictureBox^  pB_src;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::PictureBox^  pB_gra;
	private: System::Windows::Forms::Button^  btn_load;
	private: System::Windows::Forms::Panel^  panel2;
	private: System::Windows::Forms::Panel^  panel1;
	private: System::Windows::Forms::Panel^  panel3;
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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(frm_gradientfaces::typeid));
			this->label28 = (gcnew System::Windows::Forms::Label());
			this->txt_savefolder = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->cB_dataname = (gcnew System::Windows::Forms::ComboBox());
			this->pB_src = (gcnew System::Windows::Forms::PictureBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->pB_gra = (gcnew System::Windows::Forms::PictureBox());
			this->btn_load = (gcnew System::Windows::Forms::Button());
			this->panel2 = (gcnew System::Windows::Forms::Panel());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->panel3 = (gcnew System::Windows::Forms::Panel());
			this->openFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_src))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_gra))->BeginInit();
			this->panel1->SuspendLayout();
			this->panel3->SuspendLayout();
			this->SuspendLayout();
			// 
			// label28
			// 
			this->label28->AutoSize = true;
			this->label28->Location = System::Drawing::Point(38, 17);
			this->label28->Name = L"label28";
			this->label28->Size = System::Drawing::Size(58, 13);
			this->label28->TabIndex = 65;
			this->label28->Text = L"Databases";
			// 
			// txt_savefolder
			// 
			this->txt_savefolder->Location = System::Drawing::Point(110, 42);
			this->txt_savefolder->Name = L"txt_savefolder";
			this->txt_savefolder->Size = System::Drawing::Size(175, 20);
			this->txt_savefolder->TabIndex = 63;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(6, 45);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(90, 13);
			this->label3->TabIndex = 62;
			this->label3->Text = L"Save folder name";
			// 
			// cB_dataname
			// 
			this->cB_dataname->FormattingEnabled = true;
			this->cB_dataname->Items->AddRange(gcnew cli::array< System::Object^  >(4) {L"CMU-PIE", L"FERET", L"PIE", L"FEI"});
			this->cB_dataname->Location = System::Drawing::Point(110, 12);
			this->cB_dataname->Name = L"cB_dataname";
			this->cB_dataname->Size = System::Drawing::Size(137, 21);
			this->cB_dataname->TabIndex = 64;
			this->cB_dataname->SelectedIndexChanged += gcnew System::EventHandler(this, &frm_gradientfaces::cB_dataname_SelectedIndexChanged);
			// 
			// pB_src
			// 
			this->pB_src->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->pB_src->Location = System::Drawing::Point(10, 29);
			this->pB_src->Name = L"pB_src";
			this->pB_src->Size = System::Drawing::Size(200, 200);
			this->pB_src->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pB_src->TabIndex = 67;
			this->pB_src->TabStop = false;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(283, 4);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(68, 13);
			this->label1->TabIndex = 68;
			this->label1->Text = L"Gradientface";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(86, 4);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(63, 13);
			this->label2->TabIndex = 69;
			this->label2->Text = L"Originalface";
			// 
			// pB_gra
			// 
			this->pB_gra->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->pB_gra->Location = System::Drawing::Point(216, 29);
			this->pB_gra->Name = L"pB_gra";
			this->pB_gra->Size = System::Drawing::Size(200, 200);
			this->pB_gra->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pB_gra->TabIndex = 66;
			this->pB_gra->TabStop = false;
			// 
			// btn_load
			// 
			this->btn_load->Location = System::Drawing::Point(9, 78);
			this->btn_load->Name = L"btn_load";
			this->btn_load->Size = System::Drawing::Size(275, 42);
			this->btn_load->TabIndex = 70;
			this->btn_load->Text = L"Load images";
			this->btn_load->UseVisualStyleBackColor = true;
			this->btn_load->Click += gcnew System::EventHandler(this, &frm_gradientfaces::btn_load_Click);
			// 
			// panel2
			// 
			this->panel2->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"panel2.BackgroundImage")));
			this->panel2->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->panel2->Location = System::Drawing::Point(9, 126);
			this->panel2->Name = L"panel2";
			this->panel2->Size = System::Drawing::Size(275, 116);
			this->panel2->TabIndex = 116;
			// 
			// panel1
			// 
			this->panel1->Controls->Add(this->panel2);
			this->panel1->Controls->Add(this->btn_load);
			this->panel1->Controls->Add(this->label28);
			this->panel1->Controls->Add(this->txt_savefolder);
			this->panel1->Controls->Add(this->label3);
			this->panel1->Controls->Add(this->cB_dataname);
			this->panel1->Location = System::Drawing::Point(6, 12);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(295, 254);
			this->panel1->TabIndex = 117;
			// 
			// panel3
			// 
			this->panel3->Controls->Add(this->pB_src);
			this->panel3->Controls->Add(this->label1);
			this->panel3->Controls->Add(this->label2);
			this->panel3->Controls->Add(this->pB_gra);
			this->panel3->Location = System::Drawing::Point(311, 13);
			this->panel3->Name = L"panel3";
			this->panel3->Size = System::Drawing::Size(426, 252);
			this->panel3->TabIndex = 118;
			// 
			// openFileDialog
			// 
			this->openFileDialog->FileName = L"openFileDialog";
			// 
			// frm_gradientfaces
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(739, 278);
			this->Controls->Add(this->panel3);
			this->Controls->Add(this->panel1);
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"frm_gradientfaces";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Gradientfaces";
			this->Load += gcnew System::EventHandler(this, &frm_gradientfaces::frm_gradientfaces_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_src))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pB_gra))->EndInit();
			this->panel1->ResumeLayout(false);
			this->panel1->PerformLayout();
			this->panel3->ResumeLayout(false);
			this->panel3->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void frm_gradientfaces_Load(System::Object^  sender, System::EventArgs^  e) {
			 }
	private: System::Void cB_dataname_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) 
			 {
				  txt_savefolder->Text= cB_dataname->Text+"_gradientfaces";
			 }
	private: System::Void btn_load_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				System::String^ pathfolder;
				if (txt_savefolder->Text== "")
				{
					MessageBox::Show("Please put the Folder to save WASVD images","Face Recognition Application", MessageBoxButtons::OK,MessageBoxIcon::Asterisk);
					return;
				}
				else
				{
					
					pathfolder ="..\\" + txt_savefolder->Text; 

					marshal_context context;

					LPCTSTR cstr = context.marshal_as<const TCHAR*>(pathfolder);
					CreateDirectory (cstr, NULL);
				}

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
				
				int Count = 0;

				while (Count < number_of_files)
				{
					char* imgname;
					//Mat OriImg;
					OpenFileName = filename_src[Count];
					OpenFileName = OpenFileName->Replace("\\","\\\\");
					Bitmap^ OpenBmp = gcnew Bitmap(OpenFileName);
					pB_src->Image = OpenBmp;
					pB_src->Refresh();
					imgname = (char*)Marshal::StringToHGlobalAnsi(OpenFileName).ToPointer();  // convert to kieu char lay ten filecho opencv
					
					Mat img = cv::imread(imgname,0);


					Mat graface = gradientface(img);
					//normalize(graface, graface, 0, 255, CV_MINMAX);
					//graface.convertTo(graface, CV_8UC3);
					
					int row_s = (img.rows/4)*4;
					int col_s = (img.cols/4)*4;
					Mat graface_s = Mat(row_s, col_s, img.type());
					resize(graface, graface_s, graface_s.size(), 0, 0, INTER_LINEAR);

					// Display gardientface
					cv::Mat in1[] = {graface_s, graface_s, graface_s};
					cv::Mat img_temp;
					cv::merge(in1, 3, img_temp);
					//Anh mau chi can phan nay
					pB_gra->Image  = gcnew  
					System::Drawing::Bitmap(img_temp.size().width,img_temp.size().height,img_temp.step,
					System::Drawing::Imaging::PixelFormat::Format24bppRgb,(System::IntPtr) img_temp.data);
					pB_gra->Refresh();
					// save gradientface
					char* savename;
					savename = (char*)Marshal::StringToHGlobalAnsi(pathfolder + "\\" + (Count+1).ToString()+".bmp").ToPointer();
					
					imwrite(savename, graface);
                	Count++;
				}

								
			 }
};
}

