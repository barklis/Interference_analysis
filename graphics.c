#include "graphics.h"

struct BMP_Picture* load_bmp_image(const char* name)
{
	struct BMP_Picture* image = (struct BMP_Picture*) malloc(sizeof(struct BMP_Picture));
	FILE* file = fopen(name, "rb");
	if(!file)
	{
		free(image);
		perror("Error in file opening!\n");
		return NULL;
	}
	
	if(fread(image->header, 1, 54, file) != 54)
	{
		free(image);
		fclose(file);
		perror("Incorrect file!\n");
		return NULL;
	}
	
	if( image->header[0] != 'B' || image->header[1] != 'M')
	{
		free(image);
		fclose(file);
		perror("Not a .bmp file!\n");
		return NULL;
	}
	
	if( *(int*)&(image->header[0x1E]) != 0 || *(int*)&(image->header[0x1C]) != 24)
	{
		free(image);
		fclose(file);
		perror("Not a 24bpp .bmp file!\n");
		return NULL;
	} 	

	image->data_start = *(int*)&(image->header[0x0A]);
	image->size = *(int*)&(image->header[0x22]);
	image->width = *(int*)&(image->header[0x12]);
	image->height = *(int*)&(image->header[0x16]);

	if(image->size == 0)
		image->size = image->width * image->height * 3;
	if(image->data_start == 0)
		image->data_start = 54;
	
	image->data = (unsigned char*) malloc(image->size);
	fread(image->data, 1, image->size, file);
	fclose(file);

	return image;	
}

void save_bmp_image(struct BMP_Picture* image, const char* name)
{
	
}

void free_bmp_image(struct BMP_Picture* image)
{
	if(NULL != image->data)
		free(image->data);
	free(image);
}

void convert_to_bmp(const char* format, const char* name)
{

}

