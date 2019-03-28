#ifndef BMP_h
#define BMP_h

#include <unistd.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>
#include <stdio.h>
#define KADENZ 7;

using namespace std;

typedef unsigned char BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef uint64_t LWORD;
typedef vector<BYTE> BYTE_ARRAY;

struct COLOR{
    BYTE r;
    BYTE g;
    BYTE b;
};

typedef vector<vector<COLOR>> IMAGE_DATA;

struct POS_COLOR{
    int x;
    int y;
    COLOR *color;
};



COLOR BLACK{0,0,0};
COLOR WHITE{255,255,255};
COLOR RED{255,0,0};
COLOR BLUE{0,0,255};
COLOR YELLOW{255,255,0};
COLOR GREEN{0,255,0};


struct LINE{
    int start_x;
    int start_y;
    int end_x;
    int end_y;
    vector<COLOR*> data;
};

struct BLACK_LINE_X{
    COLOR *black_start;
    COLOR *black_stop;
    int y;
};


struct BLACK_LINE_Y{
    COLOR *black_start;
    COLOR *black_stop;
    int x;
};

struct WHITE_LINE_Y{
    int y;
    COLOR *white_start;
    COLOR *black_stop;
};

struct WHITE_LINE_X{
    int x;
    COLOR *white_start;
    COLOR *white_stop;
};

struct HEADER{
    BYTE head[54];
    WORD  signatur;
    DWORD file_size;
    DWORD offset_to_data;
};

struct DIBHEADER{
    DWORD HEADER;
    WORD width;
    WORD height;
    WORD bitsize;
    WORD compression;
    DWORD raw_data_size;
};

struct PIXELARRAY{
    vector<vector<COLOR>> colors;
};

struct BMP{
    HEADER head;
    DIBHEADER dibheader;
    PIXELARRAY pixelarray;
};

struct FIELD{
    vector<vector<COLOR>> field;
};

struct SUDOKU_TILE{
    int x;
    int y;
    BMP bmp;
};

struct TILE{
    POS_COLOR start;
    POS_COLOR stop;
    int length;
};

struct IMAGE_INFO{
    vector<TILE> tiles;
    long cross_sum_field;
    long cross_sum_gap;
    long exp_field_size;
    long exp_gap_size;
};

struct NUMBER_TILE{
    TILE up;
    TILE down;
    int x;
    int y;
};

struct SUDOKU_RAW_TILE{
    BMP bmp_data;
    int x;
    int y;
};
typedef vector<SUDOKU_RAW_TILE> SUDOKU_BMP_TILES;

typedef vector<vector<SUDOKU_TILE>> SUDOKU_IMAGE_TILES;


void print_black_line(BLACK_LINE_X *line){
    cout << "Y: "<< line->y << " START: "<< line->black_start <<" END: "<< line->black_stop << endl;
};

void print_black_line(BLACK_LINE_Y *line){
    cout << "X: "<< line->x << " START: "<< line->black_start <<" END: "<< line->black_stop << endl;
};

void print_pixel_array(vector<vector<COLOR>>colors){
    
    for (vector<COLOR> y : colors){
        for(COLOR color : y){
            printf("VALUES: %x %x %x\n",color.b,color.g,color.r);
        }
    }
    
};


BMP read_file(char *name){
    BMP bmp;
    FILE *file;
    file = fopen(name,"rb");
    fseek(file,0,SEEK_END);
    //--set Size
    bmp.head.file_size = (DWORD)ftell(file);
    rewind(file);
    BYTE data[bmp.head.file_size];
    fread(data,1,bmp.head.file_size,file);
    fclose(file);
    
    //HEADER INFO
    WORD d_sign = *(WORD*)&data[0];
    DWORD offset_to_data = *(DWORD*)&data[10];
    
    bmp.head.signatur = d_sign;
    bmp.head.offset_to_data = offset_to_data;
    
    //DIB HEADER
    DWORD header_size = *(DWORD*)&data[14];
    
    cout << "HEADER:"<< header_size << endl;
    WORD width = *(WORD*)&data[18];
    cout << "WIDTH: "<< width << endl;
    WORD height = *(WORD*)&data[22];
    cout << "HEIGHT: " << height << endl;
    WORD color_depth = *(WORD*)&data[28];
    cout << "BIT COUNT: " << color_depth << endl;
    WORD compression = data[32];
    cout << "Compression: " << (int)compression << endl;
    DWORD raw_data_size = bmp.head.file_size - bmp.head.offset_to_data;
    cout << "raw_data_size: " << raw_data_size <<endl;
    
    
    
    bmp.dibheader.width= width;
    bmp.dibheader.height = height;
    bmp.dibheader.bitsize = color_depth;
    bmp.dibheader.compression = compression;
    bmp.dibheader.raw_data_size = raw_data_size;
    
    
    long file_size = bmp.head.file_size;
    cout << "File Size: " <<file_size << endl;
    cout <<"----------------------------------"<<endl<<endl;
    
    long i = bmp.head.offset_to_data;
    
    for (int i_h=0;i_h < height; i_h++){
        
        vector<COLOR>arr;
        for(int i_w = 0; i_w < width;i_w++){
            COLOR tmp;
            tmp.b = data[i];
            tmp.g = data[i+1];
            tmp.r = data[i+2];
            arr.push_back(tmp);
            i = i + 3;
        }
        bmp.pixelarray.colors.push_back(arr);
    }
    
    for (long i = 0; i < bmp.head.offset_to_data;i++){
        bmp.head.head[i] =  data[i];
    }
    
    
    return bmp;
}

void build_file(BMP *bmp,char *path){
    FILE *file;
    file = fopen(path,"wb");
    BYTE toWrite;
    
    for(long i = 0;i < bmp->head.offset_to_data;i++){
        toWrite = bmp->head.head[i];
        fwrite(&toWrite, sizeof(BYTE), 1, file);
    }
    for (long y = 0; y < bmp->pixelarray.colors.size();y++){
        for (long x = 0; x < bmp->pixelarray.colors[y].size();x++){
            
            toWrite = bmp->pixelarray.colors[y][x].b;
            fwrite(&toWrite, sizeof(BYTE), 1, file);
            
            toWrite = bmp->pixelarray.colors[y][x].g;
            fwrite(&toWrite, sizeof(BYTE), 1, file);
            
            toWrite = bmp->pixelarray.colors[y][x].r;
            fwrite(&toWrite, sizeof(BYTE), 1, file);
        }
        
    }
    
};

void turn_b_w(BMP *bmp){
    
    for (long y = 0; y < bmp->pixelarray.colors.size();y++){
        for (long x = 0; x < bmp->pixelarray.colors[y].size();x++){
            
            COLOR *pixel = &bmp->pixelarray.colors[y][x];
            int sum = pixel->b + pixel->g + pixel->r;
            
            if (sum > 400){
                *pixel = BLACK;
            }else{
                *pixel = WHITE;
            }
        }
    }
};

int is_black_color(COLOR *color){
    
    if(
       (color->r != 0) or
       (color->g != 0) or
       (color->b != 0)){
        return 0;
    }else{
        return 1;
    }
};

int is_white_color(COLOR *color){
    
    if(
       (color->r != 255) or
       (color->g != 255) or
       (color->b != 255)){
        return 0;
    }else{
        return 1;
    }
};


int is_red_color(COLOR *color){
    
    if(
       (color->r == 255) &&
       (color->g == 0) &&
       (color->b == 0)){
        return 1;
    }else{
        return 0;
    }
};

int is_blue_color(COLOR *color){
    if(
       (color->r == 0) &&
       (color->g == 0) &&
       (color->b == 255)){
        return 1;
    }else{
        return 0;
    }
};

int is_green_color(COLOR *color){
    
    if(
       (color->r == 0) &&
       (color->g == 255) &&
       (color->b == 0)){
        return 1;
    }else{
        return 0;
    }
};

int is_yellow_color(COLOR *color){
    
    if(
       (color->r == 255) &&
       (color->g == 255) &&
       (color->b == 0)){
        return 1;
    }else{
        return 0;
    }
};

COLOR* get_pos(BMP *bmp,int y, int x){
    return &bmp->pixelarray.colors[y][x];
};

int call_color_functions(COLOR color,COLOR *color_ptr){
    
    if(is_white_color(&color)){
        return is_white_color(color_ptr);
    }
    if(is_black_color(&color)){
        return is_black_color(color_ptr);
    }
    if(is_blue_color(&color)){
        return is_blue_color(color_ptr);
    }
    if(is_green_color(&color)){
        return is_green_color(color_ptr);
    }
    if(is_red_color(&color)){
        return is_red_color(color_ptr);
    }
    if(is_yellow_color(&color)){
        return is_yellow_color(color_ptr);
    }
    return 0;
};


IMAGE_DATA find_all_fields(BMP *bmp){
    IMAGE_DATA fields;
    COLOR dead_pixel;
    dead_pixel.b = 1;
    dead_pixel.g = 1;
    dead_pixel.r = 1;
    
    vector <BLACK_LINE_X> black_lines_x;
    vector <vector<BLACK_LINE_X>> positions;
    
    
    for (int y = 0; y < bmp->pixelarray.colors.size();y++){
        vector<BLACK_LINE_X>tmp_blx;
        for(int x = 0; x < bmp->pixelarray.colors[y].size(); x++){
            COLOR *pixel = &bmp->pixelarray.colors[y][x];
            if(is_black_color(pixel)){
                COLOR *start_pixel = &bmp->pixelarray.colors[y][x];
                COLOR *end_pixel = &dead_pixel;
                int offset = 0;
                while(x+offset < bmp->pixelarray.colors[y].size()){
                    COLOR *w_pixel = &bmp->pixelarray.colors[y][x+offset];
                    
                    if(is_white_color(w_pixel)){
                        end_pixel = w_pixel;
                        BLACK_LINE_X blx;
                        blx.y = y;
                        blx.black_start = start_pixel;
                        blx.black_stop = end_pixel;
                        x = offset + x;
                        offset = 0;
                        tmp_blx.push_back(blx);
                        break;
                    }
                    offset++;
                }
            }
        }
        positions.push_back(tmp_blx);
    }
    
    
    for(int y = 0; y < positions.size();y++){
        for(int x = 0; x < positions[y].size();x++){
            
            BLACK_LINE_X *blx = &positions[y][x];
            
            if ( (positions[y].size() == 9) ){
                
                COLOR *ptr = blx->black_start;
                
                while( ptr != blx->black_stop){
                    ptr = ptr + 1;
                    ptr->r = 200;
                    ptr->b = 100;
                }
                
            }
        }
        
    }
    
    
    return fields;
};



void update_headers(BMP *bmp){
    
    WORD color_depth_in_byte = (WORD)bmp->head.head[28]/8;
    long height = bmp->pixelarray.colors.size();
    long width = bmp->pixelarray.colors[0].size();
    long raw_data_size = height*width*color_depth_in_byte;
    long size_of_file = raw_data_size+54;
    
    
    BYTE array[54];
    for(int i=0; i < 54;i++){
        array[i]=bmp->head.head[i];
    }
    
    bmp->head.head[5]=(size_of_file >> 24) & 0xFF;
    bmp->head.head[4]=(size_of_file >> 16) & 0xFF;
    bmp->head.head[3]=(size_of_file >> 8) & 0xFF;
    bmp->head.head[2]=size_of_file & 0xFF;
    
    bmp->head.head[21]=(width >> 24) & 0xFF;
    bmp->head.head[20]=(width >> 16) & 0xFF;
    bmp->head.head[19]=(width >> 8) & 0xFF;
    bmp->head.head[18]=width & 0xFF;
    
    bmp->head.head[25]=(height >> 24) & 0xFF;
    bmp->head.head[24]=(height >> 16) & 0xFF;
    bmp->head.head[23]=(height >> 8) & 0xFF;
    bmp->head.head[22]=height & 0xFF;
    
    bmp->head.head[37]=(raw_data_size >> 24) & 0xFF;
    bmp->head.head[36]=(raw_data_size >> 16) & 0xFF;
    bmp->head.head[35]=(raw_data_size >> 8) & 0xFF;
    bmp->head.head[34]=raw_data_size & 0xFF;
};



IMAGE_DATA cut_out_white_y(BMP *bmp){
    
    IMAGE_DATA source = bmp->pixelarray.colors;
    IMAGE_DATA cut_image;
    vector<COLOR>white_line;
    
    long img_width = source[0].size();
    long white_pxl = 0;
    long black_pxl = 0;
    
    for(int i = 0; i<img_width;i++){
        white_line.push_back(WHITE);
    }
    
    for (int y = 0; y < source.size();y++){
        for(int x = 0; x < source[y].size(); x++){
            if (is_white_color(&source[y][x])){
                white_pxl++;
            }else{
                black_pxl++;
            }
        }
        double value = (double)img_width/(double)white_pxl;
        
        if(value > 1.1){
            vector<COLOR> l = source[y];
            cut_image.push_back(l);
        }
        white_pxl=0;
    }
    
    bmp->pixelarray.colors = cut_image;
    
    return cut_image;
};

void paint_tile(TILE tile,COLOR color, BMP *bmp){
    for(int i = tile.start.x;i < tile.stop.x;i++){
        bmp->pixelarray.colors[tile.start.y][i]=color;
    }
};

IMAGE_INFO create_image_info(BMP *bmp){
    
    IMAGE_INFO img_info;
    
    long min_ex_tile = bmp->pixelarray.colors[0].size()/11;
    long max_ex_tile = bmp->pixelarray.colors[0].size()/9;
    
    vector<TILE>size_candidates;
    vector<TILE>gap_canidates;
    
    POS_COLOR start_red;
    POS_COLOR stop_blue;
    
    for(int y = 0; y < bmp->pixelarray.colors.size();y++){
        for(int x = 0; x < bmp->pixelarray.colors[y].size();x++){
            
            COLOR *color_ptr = &bmp->pixelarray.colors[y][x];
            
            if(call_color_functions(RED,color_ptr)){
                start_red.color = color_ptr;
                start_red.x = x;
                start_red.y = y;
            }
            
            if(call_color_functions(BLUE, color_ptr)){
                stop_blue.color = color_ptr;
                stop_blue.x = x;
                stop_blue.y = y;
                
                if(start_red.color){
                    TILE tile;
                    
                    tile.length = stop_blue.x - start_red.x;
                    if(tile.length>min_ex_tile&&
                       tile.length<max_ex_tile){
                    
                        tile.stop = stop_blue;
                        tile.start= start_red;
                        size_candidates.push_back(tile);
                    }
                    start_red.color = nullptr;
                    stop_blue.color = nullptr;
                }
            }
        }
    }
    
    long c_sum = 0;
    
    for(TILE t : size_candidates){
        c_sum = c_sum + t.length;
    }
    
    long cross_sum= c_sum/size_candidates.size();
   
    
    long max_gap_size = cross_sum/4;
    long min_gap_size = cross_sum/25;
    

    
    for(TILE t : size_candidates){
        POS_COLOR gap_start;
        gap_start.x = t.stop.x+1;
        gap_start.color = &bmp->pixelarray.colors[t.stop.y][t.stop.x+1];
        gap_start.y = t.stop.y;
        POS_COLOR gap_end;
        
        int i = gap_start.x;
        
        while(t.stop.x+i < bmp->pixelarray.colors[0].size()){
            
            COLOR *color_ptr = &bmp->pixelarray.colors[gap_start.y][i];
            
            if(!is_white_color(color_ptr)){
                gap_end.x = i;
                gap_end.y = t.stop.y;
                gap_end.color = color_ptr;
                
                TILE gap_tile;
                
                gap_tile.start = gap_start;
                gap_tile.stop = gap_end;
                gap_tile.length = gap_end.x-gap_start.x;
                
                if(gap_tile.length > min_gap_size&&
                   gap_tile.length < max_gap_size){
                    gap_canidates.push_back(gap_tile);
                }
                
                i++;
                
                break;
            }
            i++;
        }
    };
    
    long c_sum_gap = 0;
    for(TILE t : gap_canidates){
        c_sum_gap = c_sum_gap + t.length;
    }

    long cross_sum_gap = c_sum_gap/gap_canidates.size();
    cout << "Quersumme Gap " << cross_sum_gap<<endl;
    cout << "Quersumme Feld " <<cross_sum <<"x"<<cross_sum<<endl;
    long min_gaps = 10*cross_sum_gap;
    long min_field = 9*cross_sum;
    long min_image_width = min_field+min_gaps;
    
    if(min_image_width > bmp->pixelarray.colors[0].size()){
        cout << "BAD IMAGE"<<endl;
    }else{
        cout << "Expected Image Width: "<<min_image_width<<" Real Image Width: "<< bmp->pixelarray.colors[0].size()<<endl;
        cout << "Expected Image Height: "<<min_image_width<<" Real Image Height: "<< bmp->pixelarray.colors.size()<<endl;
    }
    
    img_info.tiles = size_candidates;
    img_info.cross_sum_field = cross_sum;
    img_info.cross_sum_gap = cross_sum_gap;
    
    return img_info;
}


IMAGE_INFO get_image_info(BMP *bmp){
    
    for(int y = 0; y < bmp->pixelarray.colors.size();y++){
        for(int x = 0; x < bmp->pixelarray.colors[y].size();x++){
            
            SUDOKU_TILE st;
            COLOR *color_ptr = &bmp->pixelarray.colors[y][x];
            COLOR *up_color_ptr = &bmp->pixelarray.colors[y+1][x];
            COLOR *down_color_ptr =&bmp->pixelarray.colors[y-1][x];
            COLOR *right_color_ptr =&bmp->pixelarray.colors[y][x+1];
            COLOR *left_color_ptr =&bmp->pixelarray.colors[y][x-1];
            
            if((x==0)||
               (y==0)||
               (y==bmp->pixelarray.colors.size()-1)||
               (x==bmp->pixelarray.colors[y].size()-1)
               ){
                continue;
            }
            
            if(
                is_black_color(color_ptr)&&
                is_white_color(up_color_ptr)&&
                is_black_color(down_color_ptr)&&
                is_white_color(left_color_ptr)&&
                is_black_color(right_color_ptr)
                ){
                color_ptr->r = 255;
            }
            
            if(
               is_black_color(color_ptr)&&
               is_white_color(up_color_ptr)&&
               is_white_color(right_color_ptr)&&
               is_black_color(down_color_ptr)&&
               is_black_color(left_color_ptr)
               ){
                color_ptr->b = 255;
            }
            
            if(
               is_black_color(color_ptr)&&
               is_black_color(up_color_ptr)&&
               is_white_color(down_color_ptr)&&
               is_white_color(left_color_ptr)&&
               is_black_color(right_color_ptr)
               ){
                color_ptr->g = 255;
               }
            
            if(
               is_black_color(color_ptr)&&
               is_black_color(up_color_ptr)&&
               is_white_color(down_color_ptr)&&
               is_black_color(left_color_ptr)&&
               is_white_color(right_color_ptr)
               ){
                color_ptr->g = 255;
                color_ptr->r = 255;
            }
        }
    }
    
    return create_image_info(bmp);
};


IMAGE_DATA get_image_data(BMP *bmp,IMAGE_INFO img_info,long offset_x, long offset_y){
    IMAGE_DATA image_data;
    IMAGE_DATA raw;
    
    for(int y = 0; y<img_info.cross_sum_field; y++){
        vector<COLOR>color_row;
        for(int x = 0; x < img_info.cross_sum_field;x++){
            COLOR color = bmp->pixelarray.colors[offset_y-y][x+offset_x];
            color_row.push_back(color);
        }
        raw.push_back(color_row);
    }
    
    for(long x = raw.size()-1;x!=0;x--){
        image_data.push_back(raw[x]);
    }
    
    return image_data;
};

SUDOKU_BMP_TILES create_tiles(IMAGE_INFO img_info,BMP *bmp){
    
    long min_x = 0;
    long max_y = 0;
    
    for(TILE img_tile : img_info.tiles){
        int x = img_tile.start.x;
        int y = img_tile.start.y;
        
        if (min_x==0){
            min_x = x;
        }
        
        if(x < min_x){
            min_x = x;
        }
        
        if(max_y==0){
            max_y = y;
        }
        
        if(y > max_y){
            max_y = y;
        }
    }
    
    
    long cross_sum_offset = img_info.cross_sum_field+img_info.cross_sum_gap;
    long x_off = min_x;
    long y_off = max_y;
    
    SUDOKU_BMP_TILES sudoku_bmp_tiles;
    
    for(int y = 0;y<9;y++){
        for(int x = 0;x<9;x++){
            SUDOKU_RAW_TILE srt;
            BMP tile_bmp;
            
            for(int i = 0;i<53;i++){
                tile_bmp.head.head[i]=bmp->head.head[i];
            }
            tile_bmp.head.offset_to_data = 54;
            
            tile_bmp.pixelarray.colors = get_image_data(bmp, img_info, x_off, y_off);
            update_headers(&tile_bmp);
            srt.bmp_data = tile_bmp;
            srt.x = x;
            srt.y = y;
            sudoku_bmp_tiles.push_back(srt);
            x_off = x_off + cross_sum_offset;
        }
        
        y_off = y_off-cross_sum_offset;
        x_off = min_x;
    }
    
    return sudoku_bmp_tiles;
}



#endif /* BMP_h */

