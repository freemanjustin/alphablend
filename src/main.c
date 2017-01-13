
// just putting this here
// has nothing to do with the content
// but this is the command to use when making movies on lust
//
// avconv -r 20 -i window%04d.png -b:v 10000k test.mp4

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "cmappr.h"


void abort_(const char * s, ...)
{
    va_list args;
    va_start(args, s);
    vfprintf(stderr, s, args);
    fprintf(stderr, "\n");
    va_end(args);
    abort();
}


void get_command_line_arg_as_string(char **variable, char *cli_input){

	*variable = malloc((strlen(cli_input)+1)*sizeof(char));
	strncpy(*variable, cli_input, strlen(cli_input));
	// fix the string
	*(&(*variable)[strlen(cli_input)]) = '\x0';
}

void get_command_line_arg_as_int(int *variable, char *cli_input){

	sscanf(cli_input,"%d",variable);
}


void alpha_blend(e *E){

    int i,j;


    for (i=0; i<E->in1.h; i++){
        for (j=0; j<E->in1.w; j++){

            E->out_image.data[(i * E->out_image.w  + j) * E->out_image.n + 0] =
                E->alpha*E->in1.data[(i * E->in1.w  + j) * E->in1.n + 0] + (1.0-E->alpha)*E->in2.data[(i * E->in2.w  + j) * E->in2.n + 0];
            E->out_image.data[(i * E->out_image.w  + j) * E->out_image.n + 1] =
                E->alpha*E->in1.data[(i * E->in1.w  + j) * E->in1.n + 1] + (1.0-E->alpha)*E->in2.data[(i * E->in2.w  + j) * E->in2.n + 1];
            E->out_image.data[(i * E->out_image.w  + j) * E->out_image.n + 2] =
                E->alpha*E->in1.data[(i * E->in1.w  + j) * E->in1.n + 2] + (1.0-E->alpha)*E->in2.data[(i * E->in2.w  + j) * E->in2.n + 2];
            E->out_image.data[(i * E->out_image.w  + j) * E->out_image.n + 3] =
                E->in1.data[(i * E->in1.w  + j) * E->in1.n + 3];
        }
    }
}



void copy_image(image *src, image *dest){

    int i, j;

    // no error checking! asusmes that the src and destination are allocated and the same size!!!!
    for(i=0;i<src->h;i++){
        for(j=0;j<src->w;j++){
            dest->data[(i * dest->w  + j) * dest->n + 0] = src->data[(i * src->w  + j) * src->n + 0];
            dest->data[(i * dest->w  + j) * dest->n + 1] = src->data[(i * src->w  + j) * src->n + 1];
            dest->data[(i * dest->w  + j) * dest->n + 2] = src->data[(i * src->w  + j) * src->n + 2];
            dest->data[(i * dest->w  + j) * dest->n + 3] = src->data[(i * src->w  + j) * src->n + 3];
        }
    }
}


int main(int argc, char **argv)
{
    e   *E;
    int i,j;
    int mode;

    int colorShift;
    int sourceChannel;
    int targetChannel;

    // malloc E;
    E = malloc(sizeof(e));
    if(E==NULL){
        fprintf(stderr,"malloc failed\n");
        exit(1);
    }

    // parse command line arguments
	if(argc < 5){
		fprintf(stderr,"args: [input file1] [input file2] [output file] [alpha level 0-100]\n");
        exit(1);
	}
	else{
        get_command_line_arg_as_string(&E->input_file, argv[1]);
        get_command_line_arg_as_string(&E->input_file2, argv[2]);
		get_command_line_arg_as_string(&E->output_file, argv[3]);
		get_command_line_arg_as_int(&E->alpha_percent, argv[4]);
	}

    // init random number generator
    srand(time(NULL));

    //open the first file
    printf("opening %s...\n", E->input_file);
    //read_png_file(argv[1]);
    // read input image
    E->in1.data = stbi_load(E->input_file, &E->in1.w, &E->in1.h, &E->in1.n, 0);

    printf("input image data: width = %d, height = %d, components = %d\n",
    E->in1.w, E->in1.h, E->in1.n);

    // open the second file
    printf("opening %s...\n", E->input_file2);
    //read_png_file(argv[1]);
    // read input image
    E->in2.data = stbi_load(E->input_file2, &E->in2.w, &E->in2.h, &E->in2.n, 0);

    printf("input image data: width = %d, height = %d, components = %d\n",
    E->in2.w, E->in2.h, E->in2.n);

    E->alpha_blend = TRUE;
    E->alpha = (float)E->alpha_percent/100.0;


    // set the run mode:
    // mode == 0 means we are alpha blending
    mode = 0;
    if(mode == 0){
        E->out_image.w = E->in1.w;
        E->out_image.h = E->in1.h;
        E->out_image.n = 4;
        E->out_image.data = (unsigned char*)malloc(E->out_image.w*E->out_image.h*E->out_image.n*sizeof(unsigned char));

        // fill the image with black
        for(i=0;i<E->out_image.h;i++){
                for(j=0;j<E->out_image.w;j++){

            E->out_image.data[(i * E->out_image.w  + j) * E->out_image.n + 0] = 0;
            E->out_image.data[(i * E->out_image.w  + j) * E->out_image.n + 1] = 0;
            E->out_image.data[(i * E->out_image.w  + j) * E->out_image.n + 2] = 0;
            E->out_image.data[(i * E->out_image.w  + j) * E->out_image.n + 3] = 0;
                }
            }

            // set the output image dimensions and the block size
            // if we are just color averaging then set the output image size to the original size

            // the original
            //block_average(E);
            // test space
            alpha_blend(E);

            // rescale image if it is too big
            // resize to 256x256 to improve color extraction performance
            // if we do this then we need to change the kmeans stuff below...
            if(E->out_image.w > 192000){
                E->scaled.w = E->out_image.w*0.25;
                E->scaled.h = E->out_image.h*0.25;
                E->scaled.n = E->out_image.n;

                E->scaled.data = (unsigned char *) malloc(E->scaled.w*E->scaled.h*E->scaled.n*sizeof(unsigned char));


                stbir_resize_uint8((const void *)E->out_image.data, E->out_image.w, E->out_image.h , 0,
                                           (const void *)E->scaled.data, E->scaled.w, E->scaled.h, 0, E->scaled.n);
                stbi_write_png(E->output_file, E->scaled.w, E->scaled.h, E->scaled.n, (const void *)E->scaled.data, 0);
            }
            else{
                stbi_write_png(E->output_file, E->out_image.w, E->out_image.h, E->out_image.n, (const void *)E->out_image.data, 0);
            }
    }

    // write output image
    //stbi_write_png(E->output_file, E->out_image.w, E->out_image.h, E->out_image.n, (const void *)E->out_image.data, 0);

    return 0;
}
