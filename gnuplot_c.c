#include "gnuplot_c.h"

int plot_data_once(const char* file_name, const char* params)
{
	FILE* plot;
	char commands[128];
	sprintf(commands, "gnuplot -p -e 'set terminal png size 400, 300; set output \"data.png\"; plot \"%s\" with %s'", file_name, params);
	plot = popen(commands, "r");
	pclose(plot);
	return 0;
}

int plot_data(const char* file_name, const char* params)
{
	
	char commands[128];
	sprintf(commands, "plot \"%s\" with %s\n", file_name, params);
	fprintf(plot.plot, "%s\n", commands);
	return 0;
}

int plot_default(void)
{
	char commands[128];
	if( NULL == plot.style )
	{
		set_style(LINES);
	}
	sprintf(commands, "plot \"%s\" with %s\n", plot.file_name, plot.style);
	fprintf(plot.plot, "%s\n", commands);
	fflush(plot.plot);
	return 0;
}

int replot_default(void)
{
	char commands[128];
	if( NULL == plot.style )
	{
		set_style(LINES);
	}
	sprintf(commands, "replot \"%s\" with %s\n", plot.file_name, plot.style);
	fprintf(plot.plot, "%s\n", commands);
	fflush(plot.plot);
	return 0;
}

int set_xlabel(const char* text)
{
	plot.x_label = (char*) malloc(strlen(text));
	strncpy(plot.x_label, text, strlen(text));
	fprintf(plot.plot, "set xlabel \"%s\"\n", plot.x_label);
	return 0;
}

int set_ylabel(const char* text)
{
	plot.y_label = (char*) malloc(strlen(text));
	strncpy(plot.y_label, text, strlen(text));
	fprintf(plot.plot, "set ylabel \"%s\"\n", plot.y_label);
	return 0;
}

int set_zlabel(const char* text)
{
	plot.z_label = (char*) malloc(strlen(text));
	strncpy(plot.z_label, text, strlen(text));
	fprintf(plot.plot, "set zlabel \"%s\"\n", plot.z_label);
	return 0;
}

int set_title(const char* text)
{
	plot.title = (char*) malloc(strlen(text));
	strncpy(plot.title, text, strlen(text));
	fprintf(plot.plot, "set title \"%s\"\n", plot.title);
	return 0;
}

int set_style(enum plot_style style)
{
	switch(style)
	{
		case LINES:
			plot.style = (char*) malloc(6);
			strncpy(plot.style, "lines", 5);
			plot.style[5] = '\0';
			break;
		case POINTS:
			plot.style = (char*) malloc(7);
			strncpy(plot.style, "points", 6);
			plot.style[6] = '\0';
			break;
		case LINESPOINTS:
			plot.style = (char*) malloc(12);
			strncpy(plot.style, "linespoints", 11);
			plot.style[11] = '\0';
			break;
		case DOTS:
			plot.style = (char*) malloc(5);
			strncpy(plot.style, "dots", 4);
			plot.style[4] = '\0';
			break;
		default:
			plot.style = (char*) malloc(6);
			plot.style[5] = '\0';
			strncpy(plot.style, "lines", 5);
	}
	return 0;
}

void send_command(const char* command)
{
	fprintf(plot.plot, "%s\n", command);
}

int load_double_data(double* data, size_t size)
{
	double* dat_buf;

	if( NULL != plot.data)
	{
		free(plot.data);
	}

	size_t i=0;
	plot.data_size = size;
	plot.data = malloc(size*sizeof(double));
	dat_buf = (double*) plot.data;

	if(NULL == plot.data)
	{
		perror("Can't allocate memory for data!\n");
		return -1;
	}

	for(i = 0; i < plot.data_size; i++)
	{
		dat_buf[i] = data[i];
	}

	return 0;
}

int load_int_data(int* data, size_t size)
{
	int* dat_buf;

	if( NULL != plot.data)
	{
		free(plot.data);
	}

	size_t i=0;
	plot.data_size = size;
	plot.data = malloc(size*sizeof(int));
	dat_buf = (int*) plot.data;

	if(NULL == plot.data)
	{
		perror("Can't allocate memory for data!\n");
		return -1;
	}

	for(i = 0; i < plot.data_size; i++)
	{
		dat_buf[i] = data[i];
	}
	
	return 0;
}


int write_double_data_tmp(void)
{
	rewind(plot.tmp_file);
	size_t i=0;
	double* dat = (double*) plot.data;

	for(i; i < plot.data_size; i++)
	{
		fprintf(plot.tmp_file,"%ld\t%lf\n", i, dat[i]);
	}
	fflush(plot.tmp_file);

	return 0;
}

int write_int_data_tmp(void)
{
	rewind(plot.tmp_file);
	size_t i=0;
	int* dat = (int*) plot.data;

	for(i; i < plot.data_size; i++)
	{
		fprintf(plot.tmp_file,"%ld\t%d\n", i, dat[i]);
	}
	fflush(plot.tmp_file);

	return 0;
}

int init_gnuplot(const char* temponary_file_name)
{
	plot.plot = NULL;
	plot.tmp_file = NULL;
	plot.extra_params = NULL;
	plot.style = NULL;
	plot.x_label = NULL;
	plot.y_label = NULL;
	plot.z_label = NULL;
	plot.title = NULL;
	plot.data = NULL;
	plot.data_size = 0;

	strncpy(plot.file_name, temponary_file_name, NAME_MAX);
	plot.tmp_file = fopen(plot.file_name, "w");
	if( NULL == plot.tmp_file )
	{
		perror("Can't create temponary file!\n");
		return -1;
	}

	char commands[NAME_MAX];	
	sprintf(commands, "gnuplot -p");

	plot.plot = popen(commands, "w");
	if( NULL == plot.plot )
	{
		perror("Can't create pipe with gnuplot!\n");
		fclose(plot.tmp_file);
		remove(plot.file_name);
		return -2;
	}

		
	return 0;	
}

void quit_gnuplot()
{
	pclose(plot.plot);
	fclose(plot.tmp_file);
	remove(plot.file_name);

	if(plot.x_label)
		free(plot.x_label);
	if(plot.y_label)
		free(plot.y_label);
	if(plot.z_label)
		free(plot.z_label);
	if(plot.style)
		free(plot.style);
	if(plot.data)
		free(plot.data);
}
