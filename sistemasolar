#include <iostream>
#include <math.h>
#include <fstream>


using namespace std;

int main (void)
{
    //Declaramos las variables

    double m[9];
    double x, y, z, denom;
    int i, j, k, e;
    double r[2][9];
    double v[2][9];
    double a[2][9];
    double aaux[2][9];
    double w[2][9];
    float h, t;
    FILE *f1, *f2;


    //Abrimos los archivos
    f1=fopen("condicionesiniciales.txt","r");
    f2=fopen("posiciontiempoplanetas.txt","w");

    //Inicializamos los vectores a 0

    for (i=0;i<=8;i++)
    for (j=0;j<=2;j++)
    {
        r[j][i]=0;
        a[j][i]=0;
        aaux[j][i]=0;
        v[j][i]=0;
    }

    //Damos valores de condiciones iniciales: masa, posición y velocidad de
    //cada planteta


    for(i=0; i<=8;i++)
    {
        fscanf(f1,"%lf\t%lf\t%lf",&x,&y,&z);
        //Leemos los datos del archivo reescalando
        m[i]=(1./1989000000000000000000000000000.)*x;
        r[0][i]=y/146600000000. ;
        v[1][i]=(0.00003357193)*z;
    }


    //Valor inicial de la aceleración

    printf("Comprobamos el valor de las aceleraciones.\n");

    for (i=1; i<=8; i++)
    {
        for(j=0; j<=8; j++)
        {
                if (j!=i)
                {
                    denom=pow((pow((r[0][i]-r[0][j]),2)+pow((r[1][i]-r[1][j]),2)),0.5);
                    a[0][i]=a[0][i]-m[j]*(r[0][i]-r[0][j])*((pow(denom, -3)));
                    a[1][i]=a[1][i]-m[j]*(r[1][i]-r[1][j])*((pow(denom, -3)));
                }

            }
        printf("%lf\t%lf\n", a[0][i],a[1][i]);
    }


h=0.02;
e=0;


    do
    {

    //Calculamos r(t+h)
    for(i=0; i<=8; i++)
    {
        for(k=0; k<=1; k++)
        {
            r[k][i]=r[k][i]+h*v[k][i]+((h*h)*0.5)*a[k][i];
        }
        //Escribimos los datos en un fichero, separados por una coma
        fprintf(f2, "%lf%c\t%lf\t%lf\t%lf\t%lf\t%lf\n", r[0][i], 44, r[1][i]), v[0][i],v[1][i], a[0][i], a[1][i];


    }
    fprintf(f2, "\n"); //Los datos tienen que estar separados en bloques

    //Calculamos a2(t+h)
    for (i=0; i<=8; i++)
    {
        for(j=0; j<=8; j++)
        {
            for(k=0; k<=1; k++)
                {
                    if (j!=i)
                    {
                        denom=((r[0][i]-r[0][j])*(r[0][i]-r[0][j])+(r[1][i]-r[1][j])*(r[1][i]-r[1][j]));
                        aaux[k][i]=aaux[k][i]-m[j]*(r[k][i]-r[k][j])*((pow(denom, -1.5)));
                    }
                }
        }
    }

    //Calculamos v(t+h)
    for(i=0; i<=8; i++)
        for(k=0; k<=1; k++)
            v[k][i]=v[k][i]+(h*0.5)*(a[k][i]+aaux[k][i]);

    //Calculamos a(t+h)
    for(i=0; i<=8; i++)
        for(k=0; k<=1; k++)
            a[k][i]=aaux[k][i];


    e=e+1;
    }
    while (e<25000);

    fclose(f1);
    fclose(f2);

    return 0;
}

