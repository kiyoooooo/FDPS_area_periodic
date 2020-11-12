#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <vector>
#include <cmath>
#include <random>
#include <string.h>      //文字列の代入に使う
#include <bits/stdc++.h> //piの利用で必要(M_PI)
//complete
class ParticleInfo
{
public:
    uint32_t id;
    uint32_t type;
    /*position*/
    double posx;
    double posy;
    double posz;
    /*velocity*/
    double velx;
    double vely;
    double velz;
    /*結合*/
    uint32_t bond_pair[2];
    uint32_t bond_type[2];
    uint32_t nbond = 0;
    /*アングル*/
    uint32_t angle_pair[2][3];
    uint32_t angle_type[2];
    uint32_t nangle = 0;
};

int main(int argc, char *argv[])
{
    std::vector<ParticleInfo> pinfo;
    ParticleInfo temp_info;
    /*
    
    
    
    
    座標の読み込みを行う．*/
    std::ifstream ifs0(argv[1]);
    if (!ifs0)
    {
        std::cerr << "error0" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    //いらないはじめの3行を捨てる．
    std::string delete_str[3];
    for (uint32_t i = 0; i < 3; i++)
    {
        std::getline(ifs0, delete_str[i]);
    }
    //ファイルの読み込み　粒子番号,粒子種は0から開始する．
    while (ifs0 >> temp_info.id >> temp_info.type >> temp_info.posx >> temp_info.posy >> temp_info.posz)
    {
        temp_info.id--;
        temp_info.type--;
        pinfo.push_back(temp_info);
    }
    ifs0.close();
    //はじめの文字列を読み込む
    double box_sx, box_sy, box_sz, box_ex, box_ey, box_ez, box_wt;
    sscanf(delete_str[0].c_str(), "'box_sx=%lf box_sy=%lf box_sz=%lf box_ex=%lf box_ey=%lf box_ez=%lf box_wt=%lf",
           &box_sx, &box_sy, &box_sz, &box_ex, &box_ey, &box_ez, &box_wt);
    //    std::cout <<std::setprecision(10)<< box_sx << " " << box_sy << " " << box_sz << " " << box_ex << " " << box_ey << " " << box_ez << " " << box_wt << std::endl;
    /*
    
    
    
    
    速度の読み込みを行う．*/
    std::ifstream ifs1(argv[2]);
    if (!ifs1)
    {
        std::cerr << "error1" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    //ファイルの読み込み
    while (ifs1 >> temp_info.id >> temp_info.velx >> temp_info.vely >> temp_info.velz)
    {
        temp_info.id--;
        pinfo.at(temp_info.id).velx = temp_info.velx;
        pinfo.at(temp_info.id).vely = temp_info.vely;
        pinfo.at(temp_info.id).velz = temp_info.velz;
    }
    ifs1.close();
    /*
    
    
    
    
    ボンド情報の読み込みを行う．粒子番号や，ボンド相手の粒子番号とボンド種が−1されていることに注意する．*/
    std::ifstream ifs2(argv[3]);
    if (!ifs2)
    {
        std::cerr << "error2" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    //ファイルの読み込み
    while (ifs2 >> temp_info.bond_pair[0] >> temp_info.bond_pair[1] >> temp_info.bond_type[0])
    {
        int i = temp_info.bond_pair[0] - 1;
        int j = temp_info.bond_pair[1] - 1;
        temp_info.bond_type[0]--;
        pinfo.at(i).bond_pair[pinfo.at(i).nbond] = j;
        pinfo.at(i).bond_type[pinfo.at(i).nbond] = temp_info.bond_type[0];
        pinfo.at(i).nbond++;

        pinfo.at(j).bond_pair[pinfo.at(j).nbond] = i;
        pinfo.at(j).bond_type[pinfo.at(j).nbond] = temp_info.bond_type[0];
        pinfo.at(j).nbond++;
    }
    ifs2.close();
    /*//bond情報をファイルに書き出す際に便利な書き方．
    for (int i = 0; i < 10; i++)
    {
        //"&&"条件で，pinfo.at(i).nbond == 1のときに重複して出力してしまうのを防ぐ．
        if (pinfo.at(i).nbond == 1 && pinfo.at(i).id < pinfo.at(i).bond_pair[0])
        {
            std::cout << pinfo.at(i).id + 1 << " " << pinfo.at(i).bond_pair[0] + 1 << " " << pinfo.at(i).bond_type[0] + 1 << std::endl;
        }
        else if (pinfo.at(i).nbond == 2)
        {
            std::cout << pinfo.at(i).id + 1 << " " << pinfo.at(i).bond_pair[1] + 1 << " " << pinfo.at(i).bond_type[0] + 1 << std::endl;
        }
    }*/
    /*
    
    
    
    
    アングル情報の読み込みを行う．粒子番号や，アングル相手の粒子番号とアングル種が−1されていることに注意する．*/
    std::ifstream ifs3(argv[4]);
    if (!ifs3)
    {
        std::cerr << "error3" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    //ファイルの読み込み
    while (ifs3 >> temp_info.angle_pair[0][0] >> temp_info.angle_pair[0][1] >> temp_info.angle_pair[0][2] >> temp_info.angle_type[0])
    {
        int i = temp_info.angle_pair[0][0] - 1;
        int j = temp_info.angle_pair[0][1] - 1;
        int k = temp_info.angle_pair[0][2] - 1;
        temp_info.angle_type[0]--;
        pinfo.at(i).angle_pair[pinfo.at(i).nangle][0] = i;
        pinfo.at(i).angle_pair[pinfo.at(i).nangle][1] = j;
        pinfo.at(i).angle_pair[pinfo.at(i).nangle][2] = k;
        pinfo.at(i).angle_type[pinfo.at(i).nangle] = temp_info.angle_type[0];
        pinfo.at(i).nangle++;

        pinfo.at(j).angle_pair[pinfo.at(j).nangle][0] = i;
        pinfo.at(j).angle_pair[pinfo.at(j).nangle][1] = j;
        pinfo.at(j).angle_pair[pinfo.at(j).nangle][2] = k;
        pinfo.at(j).angle_type[pinfo.at(j).nangle] = temp_info.angle_type[0];
        pinfo.at(j).nangle++;

        pinfo.at(k).angle_pair[pinfo.at(k).nangle][0] = i;
        pinfo.at(k).angle_pair[pinfo.at(k).nangle][1] = j;
        pinfo.at(k).angle_pair[pinfo.at(k).nangle][2] = k;
        pinfo.at(k).angle_type[pinfo.at(k).nangle] = temp_info.angle_type[0];
        pinfo.at(k).nangle++;
    }
    ifs3.close();
    /*//アングル情報をファイルに書き出す際に便利な書き方．
    for (int i = 0; i < 10; i++)
    {
        if (pinfo.at(i).nangle == 1)
        {
            std::cout << pinfo.at(i).angle_pair[0][0] + 1 << " " << pinfo.at(i).angle_pair[0][1] + 1 << " " << pinfo.at(i).angle_pair[0][2] + 1 << " " << pinfo.at(i).angle_type[0] + 1 << std::endl;
        }
    }*/
    /*


    area_eraserを開始する．
    手順
    ・ベシクルを構成する脂質のうち，一番した，もしくは一番上に存在する座標の脂質を記録する．
    ・新しいvector配列を作成する．
    ・範囲内に存在する粒子のみをコピーする．
    ・
    ・*/
    double upper_max = 0;
    double under_max = 0;
    for (int i = 0; i < pinfo.size(); i++)
    {
        if (pinfo.at(i).type + 1 == 1)
        {
            upper_max = upper_max < pinfo.at(i).posz ? pinfo.at(i).posz : upper_max;
            under_max = under_max > pinfo.at(i).posz ? pinfo.at(i).posz : under_max;
        }
    }
    //std::cout << upper_max << " " << under_max << std::endl;

    //新しい配列を生成
    const double margin = 0.2;
    std::vector<ParticleInfo> only_vesicle_pinfo;
    std::vector<ParticleInfo> double_vesicle_pinfo;
    for (int i = 0; i < pinfo.size(); i++)
    {
        if (pinfo.at(i).posz < upper_max + margin && pinfo.at(i).posz > under_max - margin)
        {
            only_vesicle_pinfo.push_back(pinfo.at(i));
        }
        if (pinfo.at(i).posz < upper_max + margin)
        {
            double_vesicle_pinfo.push_back(pinfo.at(i));
        }
    }
    for (int i = 0; i < only_vesicle_pinfo.size(); i++)
    {
        only_vesicle_pinfo.at(i).id = i;
    }
    for (int i = 0; i < double_vesicle_pinfo.size(); i++)
    {
        double_vesicle_pinfo.at(i).id = i;
    }
    //    std::cout << pinfo.size() << " " << only_vesicle_pinfo.size() << std::endl;
    /*


    area_junctionを開始する．
    手順
    ・上の手順で，double_vesicle_pinfoという，ベシクルとその下に存在する水の系を取り出してきた．
    ・double_vesicle_pinfoにonly_vesicle_pinfoを追加する形で，完成させる
    ・*/
    //box_sz = under_max - margin;
    //box_ez = upper_max + margin;
    double vesicle_sz = under_max - margin;
    double vesicle_ez = upper_max + margin;
    double vesilce_margin_rad = vesicle_sz - vesicle_ez;
    //detail.txtの記入にも必要
    double box_size_x = box_ex - box_sx,
           box_size_y = box_ey - box_sy,
           box_size_z = vesicle_ez - vesicle_sz;
    uint32_t num_water = 0, num_lipid = 0, num;
    double rho;
    for (int i = 0; i < only_vesicle_pinfo.size(); i++)
    {
        only_vesicle_pinfo.at(i).posz += box_size_z;
        only_vesicle_pinfo.at(i).id += double_vesicle_pinfo.size() + 1 - i;
        only_vesicle_pinfo.at(i).bond_pair[0] += double_vesicle_pinfo.size() + 1 - i;
        only_vesicle_pinfo.at(i).bond_pair[1] += double_vesicle_pinfo.size() + 1 - i;
        only_vesicle_pinfo.at(i).angle_pair[0][0] += double_vesicle_pinfo.size() + 1 - i;
        only_vesicle_pinfo.at(i).angle_pair[0][1] += double_vesicle_pinfo.size() + 1 - i;
        only_vesicle_pinfo.at(i).angle_pair[0][2] += double_vesicle_pinfo.size() + 1 - i;
        only_vesicle_pinfo.at(i).angle_pair[1][0] += double_vesicle_pinfo.size() + 1 - i;
        only_vesicle_pinfo.at(i).angle_pair[1][1] += double_vesicle_pinfo.size() + 1 - i;
        only_vesicle_pinfo.at(i).angle_pair[1][2] += double_vesicle_pinfo.size() + 1 - i;
        double_vesicle_pinfo.push_back(only_vesicle_pinfo.at(i));
    }

    //    box_size_z = (box_ez + (vesicle_ez - vesicle_sz) - (box_ez - vesicle_ez)) - box_sz;
    box_size_z = (box_ez - box_sz) * 2 - (box_ez - vesicle_ez) * 3;
    box_ez += (box_ez - box_sz) - (box_ez - vesicle_ez) * 3;
#if 1
    /*
    
    
    
    
    出力ファイルを生成する．*/
    //open file
    //pos_file

    FILE *fpo0;
    fpo0 = fopen(argv[5], "w");
    if (fpo0 == NULL)
    {
        printf("ERROR_initial_pos_lipid.cdv\n");
        return -1;
    }
    fprintf(fpo0, "'box_sx=%lf box_sy=%lf box_sz=%lf box_ex=%lf box_ey=%lf box_ez=%lf box_wt=%lf\n", box_sx, box_sy, box_sz, box_ex, box_ey, box_ez, box_wt);
    for (int i = 1; i < 3; i++)
    {
        fprintf(fpo0, "%s \n", delete_str[i].c_str());
    }
    for (int i = 0; i < double_vesicle_pinfo.size(); i++)
    {
        fprintf(fpo0, "%d %d   %lf   %lf   %lf \n",
                double_vesicle_pinfo.at(i).id + 1,
                double_vesicle_pinfo.at(i).type + 1,
                double_vesicle_pinfo.at(i).posx,
                double_vesicle_pinfo.at(i).posy,
                double_vesicle_pinfo.at(i).posz);
    }
    fclose(fpo0);
    /*





    */
    //vel_file
    FILE *fpo1;
    fpo1 = fopen(argv[6], "w");
    if (fpo1 == NULL)
    {
        printf("ERROR_initial_vel_lipid.cdv\n");
        return -1;
    }
    for (int i = 0; i < double_vesicle_pinfo.size(); i++)
    {
        fprintf(fpo1, "%d   %lf   %lf   %lf \n",
                double_vesicle_pinfo.at(i).id + 1,
                double_vesicle_pinfo.at(i).velx,
                double_vesicle_pinfo.at(i).vely,
                double_vesicle_pinfo.at(i).velz);
    }
    fclose(fpo1);
    /*





    */
    //bond_fileここからはofstream記法で出力する．
    std::ofstream fpo2(argv[7], std::ios::out);
    for (int i = 0; i < double_vesicle_pinfo.size(); i++)
    {
        //"&&"条件で，double_vesicle_pinfo.at(i).nbond == 1のときに重複して出力してしまうのを防ぐ．
        if (double_vesicle_pinfo.at(i).nbond == 1 && double_vesicle_pinfo.at(i).id < double_vesicle_pinfo.at(i).bond_pair[0])
        {
            fpo2 << double_vesicle_pinfo.at(i).id + 1 << "   " << double_vesicle_pinfo.at(i).bond_pair[0] + 1 << "   " << double_vesicle_pinfo.at(i).bond_type[0] + 1 << std::endl;
        }
        else if (double_vesicle_pinfo.at(i).nbond == 2)
        {
            fpo2 << double_vesicle_pinfo.at(i).id + 1 << "   " << double_vesicle_pinfo.at(i).bond_pair[1] + 1 << "   " << double_vesicle_pinfo.at(i).bond_type[0] + 1 << std::endl;
        }
    }
    fpo2.close();
    /*





    */
    //bond_file ofstream記法で出力する．
    //angle_file
    std::ofstream fpo3(argv[8], std::ios::out);
    for (int i = 0; i < double_vesicle_pinfo.size(); i++)
    {
        if (double_vesicle_pinfo.at(i).nangle == 1)
        {
            fpo3 << double_vesicle_pinfo.at(i).angle_pair[0][0] + 1 << "   " << double_vesicle_pinfo.at(i).angle_pair[0][1] + 1 << "   " << double_vesicle_pinfo.at(i).angle_pair[0][2] + 1 << "   " << double_vesicle_pinfo.at(i).angle_type[0] + 1 << std::endl;
        }
    }
    fpo3.close();
#endif
    /*





    */

    for (int i = 0; i < double_vesicle_pinfo.size(); i++)
    {
        if (double_vesicle_pinfo.at(i).type + 1 == 3)
            num_water++;
        else
            num_lipid++;
    }
    num = num_water + num_lipid;
    rho = num / (box_size_x * box_size_y * box_size_z);
    std::string filename0 = argv[9];
    std::ofstream writing_file0;
    writing_file0.open(filename0, std::ios::out);
    writing_file0 << "自分で決めるパラメータ" << std::endl;
    writing_file0 << "box_x_y = " << box_size_y / box_size_x << " (ボックスサイズのxが1としたときのyの比) " << std::endl;
    writing_file0 << "box_x_z = " << box_size_z / box_size_x << " (ボックスサイズのxが1としたときのzの比) " << std::endl;
    writing_file0 << std::endl;
    writing_file0 << "確定するパラメータ" << std::endl;
    writing_file0 << "num_water = " << num_water << " (水粒子の数) " << std::endl;
    writing_file0 << "num_lipid = " << num_lipid << " (脂質を構成する粒子の数) " << std::endl;
    writing_file0 << "num = " << num << " (すべての粒子数) " << std::endl;
    writing_file0 << "rho = " << rho << " (密度) " << std::endl;
    writing_file0 << "box_size_x = " << box_size_x << " (ボックスサイズxyz三辺が1：1：1の場合, box_size_x = box_size_y = box_size_z. ) " << std::endl;
    writing_file0 << "box_size_y = " << box_size_y << std::endl;
    writing_file0 << "box_size_z = " << box_size_z << std::endl;
    writing_file0 << std::endl;
    writing_file0 << "water : lipid = " << (double)num_water / (double)num * 100.0 << " : " << (double)num_lipid / (double)num * 100.0 << std::endl;
    writing_file0.close();

    return 0;
}