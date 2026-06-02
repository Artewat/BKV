#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define ISBIT(n,x) (((01<<(n))&(x))?1:0)
#define MAX_GLYPHS 50000
#define SIMILARITY_THRESHOLD 0.10

typedef struct {
    int w, h;
    int dx;
    int count;
    int id;
    int bytes;
    double density;
    int diam;
    int perim;
    int conn;
    unsigned char *data;
} IMG;

int popcnt8(unsigned char i) {
    int count = 0;
    while(i) { ++count; i &= i-1; }
    return count;
}

int popcnt64(unsigned long long w) {
    w -= (w >> 1) & 0x5555555555555555ULL;
    w = (w & 0x3333333333333333ULL) + ((w >> 2) & 0x3333333333333333ULL);
    w = (w + (w >> 4)) & 0x0f0f0f0f0f0f0f0fULL;
    return (int)((w * 0x0101010101010101ULL) >> 56);
}

int is_pixel_set(IMG *img, int i, int j) {
    if (i < 0 || j < 0 || i >= img->w || j >= img->h) return 0;
    return ISBIT(7 - j%8, img->data[i*(img->bytes/img->h) + j/8]);
}

IMG *load_img(int id, char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) {
        perror("Error opening file");
        return NULL;
    }

    IMG *img = (IMG *)malloc(sizeof(IMG));
    fread(&img->w, sizeof(int), 1, f);
    fread(&img->h, sizeof(int), 1, f);
    fread(&img->dx, sizeof(int), 1, f);
    fread(&img->count, sizeof(int), 1, f);
    fread(&img->id, sizeof(int), 1, f);
    fread(&img->bytes, sizeof(int), 1, f);
    
    img->data = (unsigned char *)malloc(img->bytes);
    fread(img->data, 1, img->bytes, f);
    fclose(f);
    
    return img;
}

void compute_glyph_stats(IMG *img) {
    long count = 0;
    unsigned long long *t = (unsigned long long *)img->data;
    unsigned char *s = (unsigned char *)img->data;
    int len = img->bytes;

    for (int i = 0; i < len/8; i++)
        count += popcnt64(t[i]);
    for (int i = (len/8)*8; i < len; i++)
        count += popcnt8(s[i]);

    img->count = count;
    img->density = (double)count / (img->w * img->h);

    img->perim = 0;
    int min_x = img->w, max_x = 0, min_y = img->h, max_y = 0;
    
    for (int y = 0; y < img->h; y++) {
        for (int x = 0; x < img->w; x++) {
            if (is_pixel_set(img, x, y)) {
                if (x < min_x) min_x = x;
                if (x > max_x) max_x = x;
                if (y < min_y) min_y = y;
                if (y > max_y) max_y = y;
                
                int neighbors = 0;
                neighbors += is_pixel_set(img, x-1, y);
                neighbors += is_pixel_set(img, x+1, y);
                neighbors += is_pixel_set(img, x, y-1);
                neighbors += is_pixel_set(img, x, y+1);
                
                if (neighbors < 4) img->perim++;
            }
        }
    }
    
    img->diam = (max_x - min_x) + (max_y - min_y);
    img->conn = 1;
}

double compare_glyphs(IMG *a, IMG *b) {
    if (a->w != b->w || a->h != b->h) return 1.0;
    
    int diff = 0;
    int total = a->w * a->h;
    
    for (int y = 0; y < a->h; y++) {
        for (int x = 0; x < a->w; x++) {
            if (is_pixel_set(a, x, y) != is_pixel_set(b, x, y)) {
                diff++;
            }
        }
    }
    
    return (double)diff / total;
}

void find_similar_glyphs(IMG *glyphs[], int count) {
    for (int i = 0; i < count; i++) {
        printf("Glyph %d (size %dx%d):\n", glyphs[i]->id, glyphs[i]->w, glyphs[i]->h);
        
        for (int j = i+1; j < count; j++) {
            if (glyphs[i]->w == glyphs[j]->w && glyphs[i]->h == glyphs[j]->h) {
                double similarity = compare_glyphs(glyphs[i], glyphs[j]);
                if (similarity <= SIMILARITY_THRESHOLD) {
                    printf("  Similar to glyph %d (diff %.2f%%)\n", 
                           glyphs[j]->id, similarity*100);
                }
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <glyph_files...>\n", argv[0]);
        return 1;
    }

    IMG *glyphs[MAX_GLYPHS];
    int glyph_count = argc - 1;

    for (int i = 1; i < argc; i++) {
        glyphs[i-1] = load_img(i-1, argv[i]);
        if (!glyphs[i-1]) continue;
        
        compute_glyph_stats(glyphs[i-1]);
        
        printf("Loaded %s (%dx%d): count=%d, density=%.2f, diam=%d, perim=%d\n",
               argv[i], glyphs[i-1]->w, glyphs[i-1]->h, 
               glyphs[i-1]->count, glyphs[i-1]->density,
               glyphs[i-1]->diam, glyphs[i-1]->perim);
    }

    find_similar_glyphs(glyphs, glyph_count);

    for (int i = 0; i < glyph_count; i++) {
        free(glyphs[i]->data);
        free(glyphs[i]);
    }

    return 0;
}