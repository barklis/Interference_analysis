#ifndef _GNUPLOT_C_
#define _GNUPLOT_C_

/*
 *	Maxwell, 02.10.2019
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <linux/limits.h>

enum plot_style{LINES, POINTS, LINESPOINTS, DOTS};

struct Plot{
	char file_name[NAME_MAX];
	FILE* plot;
	FILE* tmp_file;

	char* extra_params;
	//enum plot_style style_type;
	char* style;

	char* x_label;
	char* y_label;
	char* z_label;
	char* title;
	
	void* data;
	size_t data_size;
	size_t data_type;

} plot;


int plot_data(const char* file_name, const char* params);
int plot_data_once(const char* file_name, const char* params);
int plot_default(void);
int replot_default(void);

int set_xlabel(const char* text);
int set_ylabel(const char* text);
int set_zlabel(const char* text);
int set_title(const char* text);

int set_style(enum plot_style style);
void send_command(const char* command);

int load_double_data(double* data, size_t size);
int load_int_data(int* data, size_t size);

int write_double_data_tmp(void);
int write_int_data_tmp(void);

int init_gnuplot(const char* temponary_file_name);
void quit_gnuplot();


#endif
