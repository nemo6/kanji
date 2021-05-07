#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

int num = 0;
char pathx[1000] = "C:\\Users\\USERNAME\\Desktop\\Nouveau dossier (6)\\kanjidico\\bushu2\\"; // chemin dossier où se trouve les fichiers à scanner

int fsize (char *filename) { // inutilisé
    
    struct stat st;

    if ( stat(filename, &st) == 0)
    
    	return st.st_size;
}

int isDirectory (const char *path) { // vérifie si le chemin est bien un dossier
	
   struct stat statbuf;
   if (stat(path, &statbuf) != 0)
       return 0;
   return S_ISDIR(statbuf.st_mode);
	
}

int find (char *path,char *str) { // cherche pour chaque fichier présent dans le dossier un charactère spécifique

	int count = 0;
	int fcount = 0;
	char line[1000];

	if( strlen(str) != 0) {

		FILE *fp = fopen(path, "r");

		while (fgets(line, sizeof(line), fp)) { // boucle qui permet itérer pour chaque ficher présent dans le dossier

			count++;

			if(strstr(line,str)!=0){ // vérifie si le charactère spécifique est présent dans le ficher

			// printf("\n%s\n%d\n",line,count);
			fcount = count; // enregistre la ligne où se trouve le charactère spécifique
			break;

			}

		}
		fclose(fp);

	}

	return fcount;
}

char* d_line (char *path, int n) { // fonction qui renvoie le contenu de la ligne trouvé par la fonction "find" ci-dessus
    
    int count = 0;
    char line[1000];

    n = n + 12; // cas particulier : la définition du kanji se trouve 12 lignes plus bas dans chaque fichier contenu dans le dossier "bushu2"

    if( strlen(path) != 0) {

        FILE *fp = fopen(path, "r");

        while (fgets(line, sizeof(line), fp)) {

            count++;

            if (count == n){
                
                printf("%s\n",line);
            }
        
        }
        fclose(fp);
    }

}

void listdir(const char *name, int indent) { // fonction récursive : cherche tout les fichiers contenu dans un dossier, si elle tombe sur un dossier la fonction se re-éxecute elle-même pour ce dossier ( et ainsi de suite ... )
	
    DIR *dir;
    int t;
    struct dirent *entry;
    char path[1024];
    char blabla[1024];
    char cpath[1024];
    
    if (!(dir = opendir(name)))
        return;

    while ((entry = readdir(dir)) != NULL) {

        sprintf(blabla, "%s/%s", name, entry->d_name);
		
        if (entry->d_type == DT_DIR || isDirectory(blabla) ) { // vérifier que c'est un dossier

		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) // les deux dossier racines que l'on ignore
		continue;

		snprintf(path, sizeof(path), "%s/%s", name, entry->d_name); // ajoute le nom du dossier au chemin courant, pour que la fonction est la chemin complet du dossier
		listdir(path, indent + 2); // la fonction se re-éxecute avec un autre chemin dossier

        } else {

		snprintf(cpath,sizeof(cpath),"%s\%s", pathx, entry->d_name);

		t = find(cpath,"以");
		// il faut que je mette cpath et le kanji en variable argument

		if( t != 0 ) // si la fonction find à trouvé quelque chose
		d_line(cpath,t);

		num++;
		// printf("%d %*s size: [%d] %s\n", num, indent, "", fsize(blabla), entry->d_name);
        }
    }
    closedir(dir);
}

int main(void) {
	
    listdir(pathx, 0);
    return 0;
    
}
