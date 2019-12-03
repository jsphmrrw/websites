#include <stdlib.h>
#include <stdio.h>
#define STBI_ONLY_PNG
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

typedef union v3 v3;
union v3
{
	struct
	{
		float x;
		float y;
		float z;
	};
	struct
	{
		float r;
		float g;
		float b;
	};
};

static int
CStringMatchCaseInsensitive(char *a, char *b)
{
    int matches = 0;
    if(a && b)
    {
        matches = 1;
        for(int i = 0;; ++i)
        {
            if(a[i] != b[i])
            {
                matches = 0;
                break;
            }
            else if(!a[i])
            {
                break;
            }
        }
    }
    return matches;
}

static v3
ReadColorFromArguments(char **arguments, int argument_count, int start_index)
{
	v3 v = {0};
	if(start_index + 3 <= argument_count)
	{
		v.r = (float)atof(arguments[start_index+0]);
		v.g = (float)atof(arguments[start_index+1]);
		v.b = (float)atof(arguments[start_index+2]);
		arguments[start_index+0] = 0;
		arguments[start_index+1] = 0;
		arguments[start_index+2] = 0;
	}
	return v;
}

int
main(int argument_count, char **arguments)
{
	if(argument_count <= 1)
	{
		fprintf(stderr, "USAGE: %s [<--primary/-p> <r> <g> <b>] [<--secondary/-s> <r> <g> <b>] [<--highlight/-h> <r> <g> <b>]\n", arguments[0]);
	}
	else
	{
		v3 primary_color   = { 1.0f, 1.0f, 1.0f };
		v3 secondary_color = { 0.8f, 0.8f, 0.8f };
		v3 highlight_color = { 1.0f, 0.9f, 0.5f };

		for(int i = 1; i < argument_count; ++i)
		{
			if(CStringMatchCaseInsensitive(arguments[i], "--primary") ||
			   CStringMatchCaseInsensitive(arguments[i], "-p"))
			{
				arguments[i] = 0;
				primary_color = ReadColorFromArguments(arguments, argument_count, i+1);
			}
			else if(CStringMatchCaseInsensitive(arguments[i], "--secondary") ||
					CStringMatchCaseInsensitive(arguments[i], "-s"))
			{
				arguments[i] = 0;
				secondary_color = ReadColorFromArguments(arguments, argument_count, i+1);
			}
			else if(CStringMatchCaseInsensitive(arguments[i], "--highlight") ||
					CStringMatchCaseInsensitive(arguments[i], "-h"))
			{
				arguments[i] = 0;
				highlight_color = ReadColorFromArguments(arguments, argument_count, i+1);
			}
		}

		for(int i = 1; i < argument_count; ++i)
		{
			char *path = arguments[i];
			
			if(path)
			{
				int image_width = 0;
				int image_height = 0;
				int channels = 0;
				unsigned char *image = 0;

				// NOTE(rjf): Read image from file.
				{
					image = stbi_load(path, &image_width, &image_height, &channels, 4);
				}

				// NOTE(rjf): Map colors in image.
				{
					for(int j = 0; j < image_height; ++j)
					{
						for(int i = 0; i < image_width; ++i)
						{
							unsigned char *r_ptr = &image[j*image_width*4 + i*4 + 0];
							unsigned char *g_ptr = &image[j*image_width*4 + i*4 + 1];
							unsigned char *b_ptr = &image[j*image_width*4 + i*4 + 2];

							unsigned char r = *r_ptr;
							unsigned char g = *g_ptr;
							unsigned char b = *b_ptr;

							float primary_blend    = r / 255.f;
							float secondary_blend  = g / 255.f;
							float highlight_blend  = b / 255.f;

							v3 final_color =
							{
								primary_blend * primary_color.r + secondary_blend * secondary_color.r + highlight_blend * highlight_color.r,
								primary_blend * primary_color.g + secondary_blend * secondary_color.g + highlight_blend * highlight_color.g,
								primary_blend * primary_color.b + secondary_blend * secondary_color.b + highlight_blend * highlight_color.b,
							};

							*r_ptr = (unsigned char)(final_color.r * 255.f);
							*g_ptr = (unsigned char)(final_color.g * 255.f);
							*b_ptr = (unsigned char)(final_color.b * 255.f);
						}
					}
				}

				// NOTE(rjf): Output file as PNG.
				{
					char mapped_path[1024] = {0};
					snprintf(mapped_path, sizeof(mapped_path), "generated/%s", path);
					stbi_write_png(mapped_path, image_width, image_height, 4, image, image_width * 4);
				}

				stbi_image_free(image);
			}
		}
	}

	return 0;
}