
#include <math.h>
#include <fstream>


using namespace std;

int main (void)
{
    //Declaramos las variables

    double m[9];
    double x, y, z, denom, total;
    int i, j, k, e;
    double energia[9], potencial[9], periodo[9], comparar[9], suma[i];
    double r[2][9], a[2][9], v[2][9], aaux[2][9];
    float h, t;
    FILE *f1, *f2, *f3, *f4;


    //Abrimos los archivos
    f1=fopen("condicionesiniciales2.txt","r");
    f2=fopen("posiciontiempoplanetas.txt","w");
    f3=fopen("energiatiempo.txt","w");
    f4=fopen("periodos.txt","w");

    //Inicializamos los vectores a 0

    for (i=0;i<=8;i++)
    for (j=0;j<=2;j++)
    {
        r[j][i]=0.;
        a[j][i]=0.;
        aaux[j][i]=0.;
        v[j][i]=0.;
        periodo[i]=0.;
        suma[i]=0.;
        potencial[i]=0.;
        energia[i]=0.;
    }

    //Damos valores de condiciones iniciales: masa, posición y velocidad de
    //cada planteta

    printf("Las condiciones iniciales son.\n");
    printf("i         masa            r_x           r_y              v_x             v_y     \n");

    for(i=0; i<=8;i++)
    {
        fscanf(f1,"%lf\t%lf\t%lf",&x,&y,&z);
        //Leemos los datos del archivo reescalando
        m[i]=(1./1990000.)*x;
        r[0][i]=y/149.6;
        v[1][i]=0.03356227281*z;
        comparar[i]=r[1][i];
        printf("%i\t%e\t%e\t%e\t%e\t%e\n", i, m[i], r[0][i], r[1][i], v[0][i], v[1][i]);
    }


    //Valor inicial de la aceleración

    printf("Comprobamos el valor de las aceleraciones.\n");

    for (i=0; i<=8; i++)
    {
        for(j=0; j<=8; j++)
        {
                if (i!=j)
                {
                    denom=pow((pow((r[0][i]-r[0][j]),2)+pow((r[1][i]-r[1][j]),2)),0.5);
                    a[0][i]=a[0][i]-m[j]*(r[0][i]-r[0][j])*((pow(denom, -3)));
                    a[1][i]=a[1][i]-m[j]*(r[1][i]-r[1][j])*((pow(denom, -3)));
                }

            }
        printf("%e\t%e\n", a[0][i],a[1][i]);
    }

t=0.;
h=0.01;
e=0;


    do
    {
    //Calculamos r(t+h)
    for(i=0; i<=8; i++)
    {
        //Escribimos los datos en un fichero, separados por una coma
        fprintf(f2, "%e%c\t%e\n", r[0][i], 44, r[1][i]);
        for(k=0; k<=1; k++)
        {
            r[k][i]=r[k][i]+h*v[k][i]+((h*h)*0.5)*a[k][i];
            // Fuerzo que el Sol no se mueva
            r[k][0]=0.;
        }
    }
    fprintf(f2, "\n"); //Los datos tienen que estar separados en bloques



    //Calculamos aaux(t+h)
    for (i=0; i<=8; i++)
    {
        // Inicializamos a 0 en cada iteración
        aaux[0][i]=0.;
        aaux[1][i]=0.;
        for(j=0; j<=8; j++)
        {
            for(k=0; k<=1; k++)
            {
                    if (i!=j)
                    {
                        //Calculamos la aceleración auxiliar
                        denom=pow((pow((r[0][i]-r[0][j]),2.)+pow((r[1][i]-r[1][j]),2.)),0.5);
                        aaux[k][i]=aaux[k][i]-m[j]*(r[k][i]-r[k][j])*((pow(denom, -3.)));

                    }
            }

        }
        //printf("%lf\t%lf\n", aaux[0][i], aaux[1][i]);
    }

    //Calculamos v(t+h)
    for(i=0; i<=8; i++)
        for(k=0; k<=1; k++)
            v[k][i]=v[k][i]+(h*0.5)*(a[k][i]+aaux[k][i]);

    //Calculamos a(t+h)
    for(i=0; i<=8; i++)
        for(k=0; k<=1; k++)
            a[k][i]=aaux[k][i];


    total=0.;

    for (i=1; i<=8; i++)
    {
        potencial[i]=0.;
        for (j=0; j<=8; j++)
        {
            if (j!=i)
            denom=((pow((r[0][i]-r[0][j]),2.)+pow((r[1][i]-r[1][j]),2.)),0.5);
            potencial[i]=potencial[i]-m[i]*m[j]*pow(denom,-1);
        }
        //Tenemos en cuenta que todos los valores obtenidos están escalados.
        energia[i]=0.5*m[i]*(v[0][i]*v[0][i]+v[1][i]*v[1][i])+potencial[i];
    }

    //La energía total será
    for(i=1; i<=8; i++)
        total=total+energia[i];

    fprintf(f3, "%f\t%e\n", t, total);


    //Calculamos el periodo

    for (i=1; i<=8; i++)
    {
        if (periodo[i]==0) // Quiero que lo calcule una sola vez
        {
            //Aquí buscamos que la componente x de la posición sea positiva (pues es como empiezan los planetas)
            //y que la componente y sea positiva mientras que en la iteración anterior era negativa.
            //Así, obtenemos justamente el punto en el que se completa una vuelta
            //Podríamos calcular medio periodo. De esta forma necesitaríamos menos iteraciones pues el
            //último planeta solo tiene que dar media vuelta en lugar de una completa.
            if ((comparar[i]<0)&&(r[1][i]>=0))
            {
            periodo[i]=5022361.106*(1./3600)*(1./24)*(1./365.)*t; //Deshacemos el escalado
            }
            else
            comparar[i]=r[1][i];
        }
    }

    t=t+h;
    e=e+1;
    }
    while (e<20000);

    for(i=1; i<=8; i++)
        fprintf(f4,"%e\n", periodo[i]);


    fclose(f1);
    fclose(f2);
    fclose(f3);
    fclose(f4);

    return 0;
}
