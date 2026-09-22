#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void no(void)

{
  puts("Nope.");
  exit(1);
}



void xd(void)

{
  puts(
      "Iii sapores crescit rei habetur disputo. An ab istud mo prius tanta error debet. Firma foret tes mea age capax sumne. Ex ex ipsas actum culpa neque ab saepe. Existenti et principia co immittere probandam imaginari re mo. Quapropter industriam ibi cui dissimilem cucurbitas progressus perciperem. Essendi ratione si habetur gi ignotas cognitu nusquam et.Sumpta vel uti obvium hoc tribuo libere. Egisse ingens hic sed inquam tamque rum gaudet aetate dat. Sum ignem jam ullas tur nexum vul. Divelli certius si errandi accipio colores de numerum. Is quavis tangam istius lumini essent vereor ab si. Aliam rea res tango vix simul certa certi.Imponere tractatu advenire ad superest occurret se quicquam si ha. Nihil solus pappo mo ei. Tum iis rom innata gloria hos quales. Ac sequentium im sufficeret institutum ad permittere at. Aliquis aliarum quaenam at de totaque notitia ob exhibet. Simus tes sae sacra error. Neque nomen ac ad opera is reges gi nobis. Se in objectivae ab is offerendum videbuntur satyriscos. Uno sequor tritam mediam essent eae usu rea. \t\t\tActum situs ideam solum uti signa mem. De ignotas errores gi remotam invenio suppono. At argumentis facultatem attendenti explicatur transferre ob du reperiatur. Gi du mali quod fuit an unum ei. Mea sperare ego sentiat idearum spatium quaedam. Prius cur locus utrum hodie porro mente ope. Accepit liberam externo qui fal. \t\t\tVolunt illico eas animus ita odores sacras ima. De ipsa vi ad deus alio ut deum. Acquiri aliquot in liquida vi maximam is timenda. Ad aliquandiu ei facillimam repugnaret scripturas. Mearum imo namque falsae notatu hic mea non. Ero communibus exponantur hae sui quo virtutibus aliquandiu. \t\t\tNeque fieri horum errem ab me eo credo. Hanc sic meo quae ipsa. Fal membrorum existenti conservet per sapientia dubitavit. Apta gi de et enim gnum data. Id quadratam ut archimede attingere re ne. Humanam infusum has iis veteris mei occasio replere istarum. Emanant poterit capaces at in numerum de exigere ob chartam. Cui tollitur periculi cau veniebat. Communibus vi at ut p..." /* TRUNCATED STRING LITERAL */
      );
  puts(
      "Author gi ex si im fallat istius. Refutent supposui qua sim nihilque. Me ob omni ideo gnum casu. Gi supersunt colligere inhaereat me sapientia is delaberer. Rom facillimam rem expectabam rum inchoandum mei. Apertum id suppono ac generis. Ab scio ad eo deus haud meae. Hominem ex vi ut remanet at quidnam. \t\t\tTunc ullo ut anno poni voce de haud. Mallent prudens suo deumque qui sim invicem. Suum mo item inde de modi unde. Suo deo omni quia opus. Co an habent inesse semper.Et innatas dominum cogitem sperare sopitum in. Substantia dei credidisse vim iis excogitent exhibentur sub."
      );
  return;
}



void ok(void)

{
  puts("Good job.");
  return;
}


int main(void)
{
    uint curr;
    size_t arr_len;
    int char_holder;
    bool shouldKeep;
    char first_num;
    char second_num;
    char last_num;
    int local_3a;
    char user_input [24];
    char arr [9];
    uint current_index;
    int count;
    int error_check;
    int local_c;

    printf("Please enter key: ");
    error_check = scanf("%23s", user_input);
    if (error_check != 1) {
        no();
    }
    if (user_input[1] != '0') {
        no();
    }
    if (user_input[0] != '0') {
        no();
    }
    fflush(stdin);
    memset(arr,0,9);
    arr[0] = 'd';
    current_index = 2;
    count = 1;
    local_3a = 0;
      while( true ) {
        arr_len = strlen(arr);
        curr = current_index;
        shouldKeep = false;
        if (arr_len < 8) {
            arr_len = strlen(user_input);
            shouldKeep = curr < arr_len;
        }
        if (!shouldKeep) break;
        char str[4];
        str[0] = user_input[current_index];
        str[1] = user_input[current_index + 1];
        str[2] = user_input[current_index + 2];
        str[3] = '\0';
        char_holder = atoi(str);
        printf("here is the letter [%c] from the string [%s]\n ", (char)char_holder, str);
        arr[count] = (char)char_holder;
        current_index = current_index + 3;
        count = count + 1;
    }
    arr[count] = '\0';
    char_holder = strcmp(arr,"delabere");
    printf("the string %s\n", arr);
    if (char_holder == 0) {
        ok();
    }
    else {
        no();
    }

    return 0;
}