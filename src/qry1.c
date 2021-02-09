#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "retangulo.h"
#include "circulo.h"
#include "texto.h"
#include "svg.h"
#include "verificacao.h"
#include "quadtree.h"
#include "ponto.h"

void pnt(FILE* txt, QuadTree quadtrees[11], char id[], char corb[], char corp[]){
    void (*setCorb[3])(void*, char*) = {setCorbCirc, setCorbRet, setCorbTxt};
    void (*setCorp[3])(void*, char*) = {setCorpCirc, setCorpRet, setCorpTxt};
    Ponto (*getPonto[3])(void*) = {getPontoCirc, getPontoRet, getPontoTxt};
    Info fig;
    Ponto p;
    int i;
    for(i = 4; i < 7; i++){
        fig = getInfoByIdQt(quadtrees[i],id);
        if(fig != NULL){
            break;
        }
    }
    setCorb[i - 4](fig, corb);
    setCorp[i - 4](fig, corp);
    p = getPonto[i - 4](fig);
    fprintf(txt, "x: %lf y: %lf\n", getX(p), getY(p));
}

void delf(FILE* txt, QuadTree quadtrees[11], char id[]){
    QtNo node;
    Info fig = NULL;
    int i;
    double x, y, r, w, h;
    char *corb, *corp, *text, *espessura;
    for(i = 4; i < 7; i++){
        node = getInfoByIdQt(quadtrees[i],id);
        if(node != NULL){
            fig = removeNoQt(quadtrees[i], node);
            break;
        }
    }
    if(i == 4){
        x = getX(getPontoCirc(fig));
        y = getY(getPontoCirc(fig));
        r = getRCirc(fig);
        espessura = getEspessuraCirc(fig);
        corb = getCorbCirc(fig);
        corp = getCorpCirc(fig);
        fprintf(txt, "id: %s x: %lf y: %lf r: %lf espessura: %s corb: %s corp: %s\n", id, x, y, r, espessura, corb, corp);
    }
    else if(i == 5){
        x = getX(getPontoRet(fig));
        y = getY(getPontoRet(fig));
        w = getWRet(fig);
        h = getHRet(fig);
        espessura = getEspessuraRet(fig);
        corb = getCorbRet(fig);
        corp = getCorpRet(fig);
        fprintf(txt, "id: %s x: %lf y: %lf w: %lf h: %lf espessura: %s corb: %s corp: %s\n", id, x, y, w, h, espessura, corb, corp);
    }
    else if(i == 6){
        x = getX(getPontoTxt(fig));
        y = getY(getPontoTxt(fig));
        corb = getCorbTxt(fig);
        corp = getCorpTxt(fig);
        text = getTexto(fig);
        fprintf(txt, "id: %s x: %lf y: %lf corb: %s corp: %s texto: %s\n", id, x, y, corb, corp, text);
    }
}

void pontoInterno(FILE* svg, FILE* txt, QuadTree quadtrees[11], char id[], double xPonto,double yPonto){
    Info info;
    double x,y,w,h;
    char cor[10];
    info = getInfoByIdQt(quadtrees[4], id);
    if(info != NULL){
        x = getX(getPontoCirc(info));
        y = getY(getPontoCirc(info));
        h = getRCirc(info);
        if(pontoInternoCirc(xPonto,yPonto,x,y,h)){
            fprintf(txt,"%s: circulo INTERNO\n",id);
            strcpy(cor,"blue");
        }
        else{
            fprintf(txt,"%s: circulo NAO INTERNO\n",id);
            strcpy(cor,"magenta");
        }
        fprintf(svg, "\t<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" style=\"stroke: \"%s\"; stroke-width: 1\" />\n", xPonto,yPonto,x,y,cor);
        fprintf(svg,"\t<circle cx=\"%lf\" cy=\"%lf\" r=\"5\" fill=\"%s\" stroke=\"%s\"/>\n",xPonto,yPonto,cor,cor);
        return;
    }
    info = getInfoByIdQt(quadtrees[5], id);
    if(info != NULL){
        x = getX(getPontoRet(info));
        y = getY(getPontoRet(info));
        h = getHRet(info);
        w = getHRet(info);
        if(pontoInternoRet(xPonto,yPonto,x,y,w,h)){
            fprintf(txt,"%s: retangulo INTERNO\n",id);
            strcpy(cor,"blue");
        }
        else{
            fprintf(txt,"%s: retangulo NAO INTERNO\n",id);
            strcpy(cor,"magenta");
        }
        fprintf(svg, "\t<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" style=\"stroke: \"%s\"; stroke-width: 1\" />\n", xPonto,yPonto,x + w/2,y + h/2,cor);
        fprintf(svg,"\t<circle cx=\"%lf\" cy=\"%lf\" r=\"5\" fill=\"%s\" stroke=\"%s\"/>\n",xPonto,yPonto,cor,cor);
    }
}

void retanguloIntCirculo(Circulo c, Retangulo r, char idc[], char idr[], FILE* txt, FILE* svg){
    double deltaX, deltaY, x, y, w, h;
    if(getX(getPontoCirc(c)) > getX(getPontoRet(r))){
        deltaX = pow(getX(getPontoRet(r)) + getWRet(r) - getX(getPontoCirc(c)),2);
    }
    else{
        deltaX = pow(getX(getPontoRet(r)) - getX(getPontoCirc(c)),2);
    }
    if(getY(getPontoCirc(c)) > getY(getPontoRet(r))){
        deltaY = pow(getY(getPontoRet(r)) + getHRet(r) - getY(getPontoCirc(c)),2);
    }
    else{
        deltaY = pow(getY(getPontoRet(r)) - getY(getPontoCirc(c)),2);
    }
    x = menor(getX(getPontoRet(r)),getX(getPontoCirc(c)) - getRCirc(c));
    w = maior(getX(getPontoRet(r)) + getWRet(r), getX(getPontoCirc(c)) + getRCirc(c)) - x;
    y = menor(getY(getPontoRet(r)),getY(getPontoCirc(c)) - getRCirc(c));
    h = maior(getY(getPontoRet(r)) + getHRet(r), getY(getPontoCirc(c)) + getRCirc(c)) - y;
    if(sqrt(deltaX + deltaY) <=  getRCirc(c)){
        fprintf(txt,"%s: circulo %s: retangulo SIM",idc,idr);
        fprintf(svg,"<rect x='%lf' y='%lf' width='%lf' height='%lf' fill='none' stroke='black'/>\n",x,y,w,h);
    }else{
        fprintf(txt,"%s: circulo %s: retangulo NAO",idc,idr);
        fprintf(svg,"<rect x='%lf' y='%lf' width='%lf' height='%lf' fill='none' stroke='black' stroke-dasharray='4'/>\n",x,y,w,h);
    }
}

void circuloInt(Circulo c1, char idc1[], Circulo c2, char idc2[], FILE* txt, FILE* svg){
    double x,y,w,h;
    x = menor(getX(getPontoCirc(c1)) - getRCirc(c1), getX(getPontoCirc(c2)) - getRCirc(c2));
    w = maior(getX(getPontoCirc(c1)) + getRCirc(c1), getX(getPontoCirc(c2)) + getRCirc(c2)) - x;
    y = menor(getY(getPontoCirc(c1)) - getRCirc(c1), getY(getPontoCirc(c2)) - getRCirc(c2));
    h = maior(getY(getPontoCirc(c1)) + getRCirc(c1), getY(getPontoCirc(c2)) + getRCirc(c2)) - y;
    if(distancia(getX(getPontoCirc(c1)), getX(getPontoCirc(c1)), getX(getPontoCirc(c2)), getY(getPontoCirc(c2))) <= getRCirc(c2) + getRCirc(c1)){
        fprintf(txt,"%s: circulo %s: circulo SIM",idc1,idc2);
        fprintf(svg,"<rect x='%lf' y='%lf' width='%lf' height='%lf' fill='none' stroke='black'/>\n",x,y,w,h);
    }
    else{
        fprintf(txt,"%s: circulo %s: circulo NAO",idc1,idc2);
        fprintf(svg,"<rect x='%lf' y='%lf' width='%lf' height='%lf' fill='none' stroke='black' stroke-dasharray='4'/>\n",x,y,w,h);
    }
}

void retanguloInt(Retangulo r1, char idr1[], Retangulo r2, char idr2[], FILE* txt, FILE* svg){
    double x,w,y,h;
    x = menor(getX(getPontoRet(r1)),getX(getPontoRet(r2)));
    w = maior(getX(getPontoRet(r1)) + getWRet(r1),getX(getPontoRet(r2)) + getWRet(r2)) - x;
    y = menor(getY(getPontoRet(r1)),getY(getPontoRet(r2)));
    h = maior(getY(getPontoRet(r1)) + getHRet(r1),getY(getPontoRet(r2)) + getHRet(r2)) - y;
    if (w <= getWRet(r1) + getWRet(r2) && h <= getHRet(r1) + getHRet(r2)){
        fprintf(txt,"%s: retangulo %s: retangulo SIM",idr1,idr2);
        fprintf(svg,"<rect x='%lf' y='%lf' width='%lf' height='%lf' fill='none' stroke='black'/>\n",x,y,w,h);
    }
    else{
        fprintf(txt,"%s: retangulo %s: retangulo NAO",idr1,idr2);
        fprintf(svg,"<rect x='%lf' y='%lf' width='%lf' height='%lf' fill='none' stroke='black' stroke-dasharray='4'/>\n",x,y,w,h);
    }
}

void intersecao(FILE* svg, FILE* txt, QuadTree quadtrees[11], char id1[], char id2[]){
    Info fig1, fig2;
    fig1 = getInfoByIdQt(quadtrees[4], id1);
    if(fig1 != NULL){
        fig2 = getInfoByIdQt(quadtrees[4], id2);
        if(fig2 != NULL){
            circuloInt(fig1, id1, fig2, id2, txt, svg);
            return;
        }
        fig2 = getInfoByIdQt(quadtrees[5], id2);
        if(fig2 != NULL){
            retanguloIntCirculo(fig1, fig2, id1, id2, txt, svg);
            return;
        }
    }
    fig1 = getInfoByIdQt(quadtrees[5], id2);
    if(fig1 != NULL){
        fig2 = getInfoByIdQt(quadtrees[4], id2);
        if(fig2 != NULL){
            retanguloIntCirculo(fig2, fig1, id2, id1, txt, svg);
            return;
        }
        fig2 = getInfoByIdQt(quadtrees[5], id2);
        if(fig2 != NULL){
            retanguloInt(fig1, id1, fig2, id2, txt, svg);
            return;
        }
    }
}


