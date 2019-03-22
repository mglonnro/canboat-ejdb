#include <stdio.h>
#include <strings.h>
#include <ejdb/ejdb.h>

static EJDB    *jb;

#define BUFSIZE 	2048

int
main(int argc, char **argv)
{
  if (argc < 2) {
    printf("usage: canboad-ejdb <database filename>\n");
    return 1;
  }

  jb = ejdbnew();
  printf("Opening database %s\n", argv[1]);

  if (!ejdbopen(jb, argv[1], JBOWRITER | JBOCREAT)) {
    return 1;
  }
  //Get or create collection 'contacts'
  EJCOLL * coll = ejdbcreatecoll(jb, "messages", NULL);

  char            str[BUFSIZE];

  while (fgets(str, sizeof str, stdin) != NULL) {
    str[strlen(str) - 1] = 0;
    printf("Read line %s\n", str);

    bson           *bsrec;
    bson_oid_t      oid;

    bsrec = json2bson(str);
    if (bsrec == NULL) {
      fprintf(stderr, "Error converting from JSON to BSON");
    } else {

      //Save BSON
      ejdbsavebson(coll, bsrec, &oid);

      char            oid_s[25];
      bson_oid_to_string(&oid, oid_s);

      printf("\nSaved record with oid %s\n", oid_s);
      bson_destroy(bsrec);
    }
  }

  ejdbclose(jb);
  ejdbdel(jb);
  return 0;
}
