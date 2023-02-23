# GET NEXT LINE - EXAM RANK 03
## ATENTION: **THIS FUNCTION DIDN'T PASS IN THE GRADEME EXAM**, IT DOESN´T GIVE TRACEBACKS, BUT IT WORKS.

----

### **BUFFER_SIZE**

The BUFFER_SIZE is the size of the buffer used to read the file. It is defined in the get_next_line.h file.

To define the BUFFER_SIZE, we use the following expression:

```
#ifndef BUFFER_SIZE
# define BUFFER_SIZE XXX
#endif
```

XX is the size of the buffer. It is a good practice to define the BUFFER_SIZE in the Makefile.

We will have to define the BUFFER_SIZE after the definition of the header file.

-----

## **Functions**

### **ft_strlen**

The ft_strlen function returns the length of the string passed as a parameter.

```
int ft_strlen(char *str)
{
    int i;

    if (!str)
        return (0);
    i = 0;
    while (str[i])
        i++;
    return (i);
}
```

Its important to check if the string is NULL, otherwise the function may crash.



### **return_line**

The return_line function returns a line terminated by a newline from the string passed as a parameter. In this case, it will be the buffer.

```
char *return_line(char *buffer)
{
    int i;
    int j;
    char *line;

    i = 0;
    j = 0;
    while (buffer[i] != '\n' && buffer[i] != '\0')
        i++;
    if (buffer[i] == '\0')
        return (NULL);
    line = malloc(sizeof(char) * (i + 2));
    if (!line)
        return (NULL);
    while (j <= i)
    {
        line[j] = buffer[j];
        j++;
    }
    line[j] = '\0';
    return (line);
}
```

The function will return NULL if the buffer does not contain a newline.
- It's important to check if the malloc failed.
- It's important to add a '\0' at the end of the string.
- Atention to the number of characters to allocate and the number of characters to copy.
    - As we can see in the example below, we have allocated 2 more characters than the '\n' index. This is because the index starts at 0 and we need to add the '\0' at the end of the string.
    - As we can see in the example below, we used the index position INCLUSIVE. This is because we want to copy the '\n' character.
    - After the while loop, we have to add the '\0' character.



### **remove_line**

The remove_line function removes the line returned by the return_line function from the buffer.

```
char *remove_line_from_buffer(char *buffer)
{
    int i;
    int j;
    char *new_buffer;

    i = 0;
    j = 0;
    while (buffer[i] != '\n' && buffer[i] != '\0')
        i++;
    if (buffer[i] == '\0')
        return (NULL);
    new_buffer = malloc(sizeof(char) * (ft_strlen(buffer) - i + 1));
    if (!new_buffer)
        return (NULL);
    i++;
    while (buffer[i] != '\0')
    {
        new_buffer[j] = buffer[i];
        i++;
        j++;
    }
    new_buffer[j] = '\0';
    free(buffer);
    buffer = NULL;
    return (new_buffer);
}
```

The function will return NULL if the buffer does not contain a newline.
- It's important to check if the malloc failed.
- After allocating the new buffer, we copy the characters from the old buffer to the new buffer starting at the index position AFTER the '\n' character.
    - As we can see in the example below, we used the index position EXCLUSIVE. This is because we don't want to copy the '\n' character.
    - That's why we allocated 1 more character than the '\n' index, and not 2, like in the return_line function.
    - It's very important to be careful with the indexes, after allocating the new buffer, we have to copy the characters from the old buffer to the new buffer starting at the index position AFTER the '\n' character, and that's why we increment the index position AFTER the '\n' character before copying the characters.
- After the while loop, we have to add the '\0' character.
- We have to free the old buffer and set it to NULL.
  
  

### **append_buffer**

This function reallocates the buffer, allowing another read to be made.

```
char *append_buffer(char *buffer)
{
    int i;
    char *new_buffer;

    new_buffer = malloc(sizeof(char) * (ft_strlen(buffer) + BUFFER_SIZE + 1));
    if (!new_buffer)
        return (NULL);
    i = 0;
    while (buffer[i] != '\0')
    {
        new_buffer[i] = buffer[i];
        i++;
    }
    new_buffer[i] = '\0';
    free(buffer);
    buffer = NULL;
    return (new_buffer);
}
```

- To achieve this, we have to allocate a new buffer with the size of the old buffer + the BUFFER_SIZE + 1.
    - The BUFFER_SIZE is the size of the buffer used to read the file.
    - The + 1 is for the '\0' character.
- It's important to check if the malloc failed.
- After allocating the new buffer, we copy the characters from the old buffer to the new buffer.
- The index position here are simple, we start at 0 and we copy the characters until we reach the '\0' character.
- After the while loop, we have to add the '\0' character, as always.
- We then free the old buffer and set it to NULL.


### **get_next_line**

The get_next_line function returns a line terminated by a newline from the file descriptor passed as a parameter.

```
char *get_next_line(int fd)
{
    static char *buffer = NULL;
    char *line = NULL;
    int i;

    if (fd < 0 || BUFFER_SIZE <= 0)
        return (NULL);
    if (!buffer)
    {
        buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
        if (!buffer)
            return (NULL);
    }
    i = 0;
    while (buffer[i] != '\0' && buffer[i] != '\n')
        i++;
    if (buffer[i] == '\0')
    {
        buffer = append_buffer(buffer);
        if (!buffer)
            return (NULL);
        int len = ft_strlen(buffer);
        i = read(fd, buffer + ft_strlen(buffer), BUFFER_SIZE);
        buffer[len + i] = '\0';
        if (i == -1)
            return (NULL);
        buffer[ft_strlen(buffer)] = '\0';
        if (i == 0)
            return (NULL);
        return (get_next_line(fd));
    }
    line = return_line(buffer);
    buffer = remove_line_from_buffer(buffer);
    return (line);
}
```

- It's important to check if the file descriptor is valid and if the BUFFER_SIZE is valid, so we don't have any unexpected behavior.
- First, we check if the buffer is NULL. If it is, we allocate the buffer.
    - It's important to check if the malloc failed.
- Then, we check if the buffer contains a newline. If it does, we return the line.
    - We use the return_line function to return the line.
    - We use the remove_line_from_buffer function to remove the line from the buffer.
- If the buffer does not contain a newline, we read the file descriptor and append the buffer.
    - We use the append_buffer function to append the buffer.
    - It's important to check if the malloc failed.
    - We read the file descriptor and append the buffer.
    - It's important to check if the read failed.
    - We check if the read returned 0. If it did, we return NULL.
    - If the read did not return 0, we call the get_next_line function again.
- If the buffer contains a newline, we return the line.
    - We use the return_line function to return the line.
    - We use the remove_line_from_buffer function to remove the line from the buffer.
We use recursion to read the file descriptor and append the buffer until we find a newline.

-----

## **Main**

```
#include <fcntl.h>

int main(void)
{
    int fd = open("test", O_RDONLY);
    char *line = NULL;

    printf("%s", line = get_next_line(fd));
    close(fd);
    return (0);
}
```

----

## **Example test file**

```
Ceci est un test de GNL.

Si vous voyez ceci, vous avez échoué à l'exercice précédent.
Respirez.
Faites des tests simple pour vous assurer que vous avez bien compris ce que vous avez fait.

Vous devez compiler avec -D BUFFERSIZE=x
evitez de define la taille du buffer dans votre .c

Lisez et relisez votre sujet, il n'est pas la pour rien.

Voici un extrait wikipedia qui pourrait vous aider : 

Cornichon
Cucumis sativus

Classification phylogénétique
Classification phylogénétique
Ordre	Cucurbitales
Famille	Cucurbitaceae
Le cornichon est une plante herbacée annuelle de la famille des Cucurbitaceae, cultivée pour ses fruits, récoltés avant maturité et consommés principalement comme condiment.
Le nom scientifique est Cucumis sativus, le même que celui du concombre (ils sont tous deux de la même espèce). Historiquement, le concombre était récolté à maturité, à la différence du cornichon, puis des variétés ont été sélectionnées pour produire des cornichons et d'autres des concombres. Les sélections effectuées par l'homme ont accentué les caractères propres à chacun, pour obtenir des cultivars distincts1.

Sommaire
1	Variétés
2	Culture
3	Production
4	Différents goûts, tailles et aspects
4.1	Cornichons au vinaigre
5	Lexicographie
6	Cornichon dans la culture populaire
7	Notes et références
8	Voir aussi
8.1	Articles connexes
Variétés[modifier | modifier le code]
Près de 370 variétés sont inscrites au Catalogue européen2 et près de 10 au Catalogue français3.
Quelques variétés : Ceto, de Bourbonne, de Russie, Faster, Fin de Meaux, Parigyno, Silver, Vert petit de Paris...
Culture[modifier | modifier le code]
Le sol doit être profond, meuble et bien fertilisé, l'exposition bien ensoleillée.
Le semis se fait en serre ou en pleine terre (T° supérieure à 16+) quand les gelées ne sont plus à craindre. Le semis se fait en poquet de 2 à 3 graines à 50 cm sur le rang et à 1 m entre les lignes, généralement palissées. Seul le plant le plus vigoureux est conservé.
Sensible au froid, il est conseillé de mettre un voile de forçage en période froide au printemps.
Il est conseillé de pailler et d'irriguer sans mouiller les feuilles.
Il est inutile de tailler les cornichons et la récolte se fait au fur et à mesure.
Les principales maladies sont le mildiou et l'oïdium. Les limaces, les escargots, les mouches blanches, les araignées rouges, le puceron noir, peuvent faire des dégâts importants.
Production[modifier | modifier le code]
Production en tonnes de concombre et de cornichon. Chiffres 2003-2004
Données de FAOSTAT (FAO)
Chine	25 059 864	64 %	25 558 000	64 %
Turquie	1 750 000	4 %	1 750 000	4 %
Iran	1 350 000	3 %	1 350 000	3 %
États-Unis	1 046 960	3 %	1 046 960	3 %
Fédération de Russie	697 000	2 %	715 000	2 %
Japon	683 000	2 %	700 000	2 %
Égypte	630 000	2 %	630 000	2 %
Indonésie	514 210	1 %	514 210	1 %
Ukraine	515 000	1 %	460 000	1 %
Corée du Sud	445 033	1 %	440 000	1 %
Espagne	440 000	1 %	440 000	1 %
Mexique	435 000	1 %	435 000	1 %
Pays-Bas	430 000	1 %	429 000	1 %
Autres pays	5 465 982	14 %	5 371 934	14 %
Total	39 461 049	100 %	39 840 104	100 %
Différents goûts, tailles et aspects[modifier | modifier le code]
Les cornichons que l'on trouve généralement commercialisés en France aujourd’hui, généralement importés, principalement de Chine, du sud de l’Inde et du Viet Nam, sont petits, craquants. Cependant, les cornichons peuvent avoir un goût, une taille et un aspect différents. Ainsi, en Europe centrale et orientale, les cornichons sont souvent plus gros et plus doux, tel le malossol, emblématique des cuisines slaves. Notons qu’en Russie, bien que le mot cornichon (корнишон) existe, on utilise habituellement le mot concombre, « ogouriéts » (огурец). Il y a aussi des variétés sans boursouflures, que l'on trouve notamment en Angleterre et en Amérique du Nord.
Cornichons au vinaigre[modifier | modifier le code]
Les cornichons frottés et parfois dessalés au gros sel sont conservés confits dans le vinaigre blanc et le sel, condimentés à l'estragon, l'échalote, l'ail, les graines de capucine et souvent en compagnie de petits oignons blancs4,5,6. La macération dans un vinaigre versé bouillant dure 15 jours7. Coupés en julienne ou en tranches ils sont la finition indispensable de la sauce charcutière qui se mange chaude et hachés dans la mayonnaise de la sauce tartare (avec œuf dur, câpres, échalote et persil) et de la sauce verte8.
Les cornichons confits au vinaigre font merveille dans les sandwichs (en premier le jambon/beurre), et les gros cornichons doux (Dill Pickles) dans les hamburgers9,10.
Lexicographie[modifier | modifier le code]
Le mot « cornichon », à l'origine « petite corne », est également utilisé en langage familier pour désigner une personne niaise, stupide11.
L’appellation « cornichon de mer » est parfois utilisée pour désigner la salicorne d'Europe, une plante d’aspect fort différent des cornichons, qui n’est pas botaniquement de la même famille, et qui ne doit ce surnom qu’à cause d’un usage culinaire similaire. Quand elle est jeune, sa tige peut être confite dans le vinaigre et servir de condiment, à la manière d’un cornichon.
Le terme « cornichon » était utilisé par Vauban pour désigner une fortification comportant deux bastions reliés par une courtine comme, par exemple, au château de Guillaumes.
Par ailleurs, le bâtiment du 30 St Mary Axe à Londres, en forme d'obus, est surnommé Gherkin qui signifie cornichon.
C’est aussi le surnom donné à tout élève de classe préparatoire à l’École spéciale militaire de Saint-Cyr, peut-être par analogie avec l'idée d'un entassement de ces élèves durant leurs études dans un « bocal » (surnom de l'école). La dite classe est appelée elle-même corniche.
Cornichon dans la culture populaire[modifier | modifier le code]
Dans la série d'animation Rick et Morty, Rick décide de se transformer en cornichon dans l'épisode Rick-ornichon.
« Les Cornichons » est une chanson de Nino Ferrer (1973), racontant un pique-nique dans la nature.
Amora invente en 1987 le TIRE CROQ, qui permet de prendre les cornichons sans mettre les doigts dans le vinaigre. Un petit panier en plastique arrivé sur le marché après une idée original d’un publicitaire nommé Maurice Fournet.

Voici un extrait de la chanson « Les Cornichons » de Nino Ferrer :

On est parti, samedi, dans une grosse voiture
Faire tous ensemble un grand pique-nique dans la nature
En emportant des paniers, des bouteilles, des paquets
Et la radio!
Des cornichons
De la moutarde
Du pain, du beurre
Des p'tits oignons
Des confitures
Et des œufs durs
Des cornichons
Du corned-beef
Et des biscottes
Des macarons
Un tire-bouchon
Des petits-beurre
Et de la bière
Des cornichons
On n'avait rien oublié, c'est maman qui a tout fait
Elle avait travaillé trois jours sans s'arrêter
Pour préparer les paniers, les bouteilles, les paquets
Et la radio!
Le poulet froid
La mayonnaise
Le chocolat
Les champignons
Les ouvre-boîtes
Et les tomates
Les cornichons
Mais quand on est arrivé, on a trouvé la pluie
C'qu'on avait oublié, c'était les parapluies
On a ramené les paniers, les bouteilles, les paquets
Et la radio!
On est rentré
Manger à la maison
Le fromage et les boîtes
Les confitures et les cornichons
La moutarde et le beurre
La mayonnaise et les cornichons
Le poulet, les biscottes
Les œufs durs et puis les cornichons
```
