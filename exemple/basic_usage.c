/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   basic_usage.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qloubier <qloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/09 20:20:44 by qloubier          #+#    #+#             */
/*   Updated: 2017/04/10 00:13:18 by qloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include <time.h>

/*---------------------------------------------------------------
**
** La commande pour compiler le fichier depuis le répertoire principale
** de la MGLW est :
** make && clang -o expl exemple/basic_usage.c -Iinclude -L. -lmglw `make static-libs`
**
** Cette exemple montre comment utiliser la MGLW pour ouvrire une fenêtre,
** récupérer les entrées utilisateur pour la souris et le clavier, et dessiner
** dans une image afficher à l'écran.
**
** Commencer par la fonction main à la fin du fichier pour mieux comprendre.
**
**-------------------------------------------------------------*/

// On importe la MGLW
#include "mglw/mglw.h"

# define FULLSCREEN	0x01u
# define DRAW		0x02u
# define RED		0xFFu
# define GREEN		0xFF00u
# define BLUE		0xFF0000u

// Structure "context" contenant les données principales
typedef struct		s_context
{
	// Variables pour stocker la paramètres récupérés par
	// les events clavier et souris
	unsigned int	flags;
	unsigned int	color;
	int				mouse[2];
	// Structure pour stocker la fenêtre
	mglwin			*win;
}					t_ctx;

// Gestion des touches
int		keypress(void *c, int k)
{
	t_ctx			*ctx;

	// On récupère le context
	ctx = (t_ctx *)c;
	printf("key %i pressed\n", k);
	// Active le mode dessin
	if (k == MGLW_MOUSE_BUTTON_1)
		ctx->flags |= DRAW;
	return (0);
}

int		keyrelease(void *c, int k)
{
	t_ctx			*ctx;

	// On récupère le context
	ctx = (t_ctx *)c;
	printf("key %i released\n", k);
	// On active/désactive le mode fullscreen
	if (k == MGLW_KEY_F)
	{
		ctx->flags ^= FULLSCREEN;
		mglw_togglefullscreen(ctx->win, 1);
	}
	// Ajout/Supression du rouge dans la couleur du curseur
	else if (k == MGLW_KEY_R)
		ctx->color ^= RED;
	// Ajout/Supression du vert dans la couleur du curseur
	else if (k == MGLW_KEY_G)
		ctx->color ^= GREEN;
	// Ajout/Supression du bleu dans la couleur du curseur
	else if (k == MGLW_KEY_B)
		ctx->color ^= BLUE;
	// On stop la fenêtre quand Q est appuyé
	// !! EN AZERTY Q == "A" !!
	else if (k == MGLW_KEY_Q)
		mglwin_stop(ctx->win);
	// Désactive le mode dessin
	else if (k == MGLW_MOUSE_BUTTON_1)
		ctx->flags &= ~DRAW;
	return (0);
}

// Gestion de la souris
int		mouse_move(void *c, double x, double y)
{
	t_ctx			*ctx;

	// On récupère le context
	ctx = (t_ctx *)c;
	// On stock la position de la souris dans le context
	ctx->mouse[0] = (int)x;
	ctx->mouse[1] = (int)y;
	// Décomenter cette ligne pour voir les valeurs x et y
	// printf("move %f, %f\n", x, y);
	return (0);
}

// Gestion de la molette
int		mouse_wheel(void *c, double x, double y)
{
	// Décomenter cette ligne pour voir les valeurs x et y
	// printf("wheel %f, %f\n", x, y);
	return (0);
}

// Gestion du resize de la fenêtre
void	resizeprocess(void *c, int w, int h)
{
	printf("new window size %i, %i\n", w, h);
}

// Fonction qui dessine un carré de 5 pixels de coté au coordonnés indiquées
void	setpx(mglimg *img, int x, int y, unsigned int col)
{
	int		sx;
	int		sy;

	sx = 5;
	x -= 3;
	y -= 3;
	while (sx--)
	{
		sy = 5;
		while (sy--)
		{
			y++;
			if ((x < 0) || (y < 0) || (x >= img->x) || (y >= img->y))
				continue ;
			((unsigned int *)(img->pixels))[x + (y * img->x)] = col;
		}
		y -= 5;
		x++;
	}
}

int		main()
{
	// Structure pour représenter une durée pour le nanosleep, ~0.012 sec
	const struct timespec	t = (struct timespec){0, 12000000L};
	// Structure principale
	t_ctx					ctx;
	// Structure pour stocker une image
	mglimg					*img;

	ctx.flags = 0;
	ctx.color = 0xFF000000u;
	// On initialise la MGLW et exit en cas d'erreur
	if (!mglw_init())
		return (-1);
	// On change les paramètres généraux de la MGLW
	mglw_setsetting(MGLWS_EXITKEY, MGLW_KEY_ESCAPE);
	// On crée un objet de type fenêtre, mais elle n'est pas encore ouverte
	// La fonction prends en paramètre le mode de rendu (pour l'instant seul
	// legacy est disponible) et les flags
	// Le flag 2DLAYER permet d'initialiser une image de la taille de
	// la fenêtre et qui sera afficher automatiquement
	// !! La fonction peut renvoyer un NULL !!
	ctx.win = mglw_mkwin(MGLW_LEGACY_MODE, MGLW_2DLAYER);
	// On ouvre la fenêtre crée précédemment,
	// la fonction est protégé contre le NULL pour "ctx.win"
	// !! La fonction peut renvoyer un NULL !!
	ctx.win = mglw_openwin(ctx.win, 800, 600, "Titre de la fenetre");
	// On vérifie si la fenêtre à pu être créée et ouverte et on exit
	// dans le cas contraire
	if (!ctx.win)
	{
		mglw_close();
		return (-1);
	}
	// On récupère le 2DLAYER et si il n'exist pas on exit
	// !! ATTENTION !! Lors d'un passage au fullscreen le 2D layer sera perdu;
	// Il est prévu pour être redessiné à chaque frame.
	img = (mglimg *)mglw_get2dlayer(ctx.win);
	if (!img)
	{
		mglw_close();
		return (-1);
	}
	// On vide le 2DLAYER pour qu'il soit entièrement noir et transparent
	bzero(img->pixels, img->memlen);
	// On ajoute les fonction callback pour les events
	// de type boutons (keyboard et mouse button)
	// Le second paramètre défini l'état de l'event en question :
	// 0 : Relaché
	// 1 : Viens d'être appuyé
	// 2 : Répétition après appui
	mglw_setkcb(ctx.win, 1, &keypress, &ctx);
	mglw_setkcb(ctx.win, 0, &keyrelease, &ctx);
	// On ajoute les fonction callback pour la souris
	mglw_setmcb(ctx.win, 0, &mouse_move, &ctx);
	mglw_setmcb(ctx.win, 1, &mouse_wheel, &ctx);
	// On ajoute les fonction callback pour le resize de la fenêtre
	mglw_setsizecb(ctx.win, &resizeprocess, &ctx);
	// On boucle tant que la fenêtre est ouverte et considéré comme active
	while (mglwin_run(ctx.win))
	{
		// Si le flag DRAW est actif (clic gauche enfoncé), on dessine un carré
		// de pixels
		if (ctx.flags & DRAW)
			setpx(img, ctx.mouse[0], ctx.mouse[1], ctx.color);
		// On mets le programme en attente pendant quelques miliseconde
		// pour éviter d'utilise 100% du cpu constament
		nanosleep(&t, NULL);
	}
	mglw_close();
	return (0);
}
