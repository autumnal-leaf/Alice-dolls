#include "comformity.h"

void main () {
    int n; // Number of Segment
    char angleMode; // Angle calculation model
    int OO_Angle[128][3]; // Traverse Point.Obervation_Angle(D M S)
    int CA_Angle[128][3]; // Computative Azimuth Angle(D M S)
    double dis[128]; // Traverse Point.Distance
    double C_Coord[2][2]; // Control Point.Coord(x y)
    double D_Coord[128][2]; // Coordinate increment
    double T_Coord[128][2]; // Traverse Point Coord(x y)

    int preAzimuth[2][3]; // Control edge azimuth(D M S)
    int SumEr_Angle[3]; // The sum of error angle(D M S)
    double Er_Coord[3];  // X,Y Coordinate Closure Error

    FILE *fp1,*fp2;
    fp1 = fopen("DATA.txt","r");
    fp2 = fopen("RES.txt","w");

    fprintf(fp2,"AUTO WIRE-Third Edition\nDev by autumnal_leaf\n");
    printf("Start reading the data file......\n");

    // Start to read the data file
    fscanf(fp1, "Number of Segment:%d\n", &n);
    fscanf(fp1,"Angle calculation model( L or R ):%c\n",&angleMode);
    printf("test");
    // Skip the meanless info    
    Skip(fp1);
    
    // Read in the coordinate of the initial edge and end edge
    fscanf(fp1,"Point A:%lf,%lf\n", &C_Coord[0][0], &C_Coord[0][1]);
    fscanf(fp1,"Point B:%lf,%lf\n", &T_Coord[0][0], &T_Coord[0][1]);
    Skip(fp1);
    fscanf(fp1,"Point C:%lf,%lf\n", &T_Coord[n-1][0], &T_Coord[n-1][1]);
    fscanf(fp1,"Point D:%lf,%lf\n", &C_Coord[1][0], &C_Coord[1][1]);

    // Skip the meanless info
    Skip(fp1);

    // Readin Traverse Point: No/Obervation_Angle(D M S)/Distance_From_Last
    for( int intBuffer = 0,i = 0 ; i < n; i++){
    fscanf(fp1,"%d/%d %d %d/%lf\n", &intBuffer, &OO_Angle[i][0], &OO_Angle[i][1], &OO_Angle[i][2], &dis[i]);
    }

    printf("Finish reading, start calculating......\n");

    // Calculate the azimuth of the start edge and the end edge
    Angle_Egde( C_Coord[0], T_Coord[0], preAzimuth[0]);
    Angle_Egde( T_Coord[n-1], C_Coord[1], preAzimuth[1]);

    // Calculate the azimuth of each segment with ordinary data
    Angle_cul( preAzimuth[0], OO_Angle[0], CA_Angle[0], angleMode); // Calculate the azimuth of point B
    for( int i = 1; i < n-1; i++){
        Angle_cul(CA_Angle[i-1], OO_Angle[i], CA_Angle[i], angleMode);
    }
    Angle_cul( CA_Angle[n-2], OO_Angle[n-1], CA_Angle[n-1], angleMode); // Calculate the azimuth of point C
    
    // Calculate the angular closure error and check
    Angle_AS( CA_Angle[n-1], preAzimuth[1], SumEr_Angle, '-');
    if ( abs(SumEr_Angle[0]*3600 + SumEr_Angle[1]*60 + SumEr_Angle[2]) > (40*sqrt(n)) ) {
        printf("The angular closure error exceeds the limit !\n");
    }

    // Correcting the value
    Correct_Azimuth(OO_Angle, OO_Angle, SumEr_Angle, n, angleMode);

    // Calculate the azimuth with crooected value
    for( int i = 1; i < n-1; i++){
        Angle_cul(CA_Angle[i-1], OO_Angle[i], CA_Angle[i], angleMode);
    }
    
    // Calculate the coord increment
    // Form segment 0 to segment n-1
    for( int i = 0; i < n-1; i++){
        Coord_Increment( &dis[i], CA_Angle[i], &D_Coord[i][0], 'x');
        Coord_Increment( &dis[i], CA_Angle[i], &D_Coord[i][1], 'y');
    }

    // Calculate and correct the coodinate closure error
    Coordinate_Error( n, D_Coord, T_Coord[0], T_Coord[n-1], Er_Coord, dis);


    // Calculate the coords with crooected value
    for( int i = 1; i < n; i++){
        T_Coord[i][0] = T_Coord[i-1][0] + D_Coord[i-1][0];
        //printf("%lf\n",( -xyEr_Coord[0] * ( dis[i] / F_sum( dis, n+1) ) ) );
        T_Coord[i][1] = T_Coord[i-1][1] + D_Coord[i-1][1];
        //printf("%lf\n",( -xyEr_Coord[1] * ( dis[i] / F_sum( dis, n+1) ) ) );
    }
    
    // File output
    printf("File output......\n");

    fprintf(fp2,"////////Precision parameter////////\n");
    fprintf(fp2,"Angular closure error:%d\" \n", (SumEr_Angle[0] * 3600 + SumEr_Angle[1] * 60 + SumEr_Angle[2]) );
    fprintf(fp2,"Closure error of conductor:\nfx = %.4lfm   fy = %.4lfm\n", Er_Coord[0], Er_Coord[1]);
    fprintf(fp2,"fs = %.4lfm   ",Er_Coord[2]);
    fprintf(fp2,"k = %lf\n", Er_Coord[2] / F_sum( dis, n-1) );
    fprintf(fp2,"////////The result of the calculation of the wire points////////\nNo/Azimuth_Angle(D M S)/Coord(x y)\n");
    for( int i =0; i < n; i++){
    fprintf(fp2,"%d/%d %d %d/%.4lf %.4lf\n", i, CA_Angle[i][0], CA_Angle[i][1], CA_Angle[i][2], T_Coord[i][0], T_Coord[i][1]);
    }

    fclose(fp1);
    fclose(fp2);


}