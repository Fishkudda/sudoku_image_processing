#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>
#include "Number.h"
#include "Field.h"
#include "BMP.h"


int main(int argc, const char * argv[]) {
    
    if(argv[1]){
        
        try{
            char path[80];
            stpcpy(path, argv[1]);
            BMP bmp = read_file(path);
            turn_b_w(&bmp);
            cut_out_white_y(&bmp);
            update_headers(&bmp);
        
            IMAGE_INFO img_info = get_image_info(&bmp);
            turn_b_w(&bmp);
            SUDOKU_BMP_TILES sbt = create_tiles(img_info, &bmp);
        
            for(auto data:sbt){
                BMP *bmp_ptr=&data.bmp_data;
                char buff[40];
                
                int x = data.x ;
                int y = data.y ;
            
                snprintf(buff, sizeof(buff), "%s%d_%d", "tiles/",x,y);
                build_file(bmp_ptr,buff);
            }
        }catch(exception e){
            cout << "ERROR WHILE CONVERTING" << endl;
            return EXIT_FAILURE;
        }
        
        
    }else{
        printf("Needs Argument, path to file.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}


