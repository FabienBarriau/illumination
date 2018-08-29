# illumination

automaticExtract Permet d'extraire automatiquement les palette de couleurs. 

1. Recadrage de la moitié haute de l'image, le seuil est choisi manuellement.
2. Seuillage adaptatif (otsu)
3. Calcul de la plus grande surface connexe qui, dans ce cas, est la palette de couleurs.
4. Repérage des coins du rectangle, calcul de l'homographie entre ces points et les coins de la palette de couleurs de référence.
5. Recalage de la palette de couleurs sur le modèle, grâce à l'homographie calculée précédemment.
6. Extraction des couleurs grâce aux repères sur le modèle.

Des erreurs peuvent survenir à cause du seuillage initial, si celui ci n'est pas précis l'algorithme donnera un mauvais résultat, voici quelques exemples de perturbation pouvant causer un "mauvais" seuillage.

![Alt](/error/example1.png "erreur 1")
![Alt](/error/example2.png "erreur 2")

Si l'extraction automatique ne fonctionne pas, il faut utiliser manuelExtract.

Celui-ci réalise les mêmes tâches que sa version automatique sauf le seuillage. C'est l'utilisateur qui doit fournir un masque binaire avec la palette de couleur en blanc et le reste en noir.

Après l'extraction il est possible grâce à colorcorrection d'appliquer la transformation couleur pour homogéiniser les illumination des différentes photos.

Pour corriger l'illumination, seulement l'échelle de gris en bas de la palette est utilisé. Les coefficient de la  régression linéaire des valeurs du canal bleu des niveaux de gris sur ceux de la palette référence est calculé. Puis la transformation inverse est appliqué Cette opération est aussi réalisé pour les canaux rouge et vert.

FLV contient un exemple de structure répertoire avec lequel les algo fonctionne et les résultats de l'extraction + correction pour quelques photos.
