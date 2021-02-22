//---------------------------------------------------------------
// UTN-FRBB
// Informática II
// Prof.: Ing. G. Friedrich
// Ejemplo básico de programa con GUI basado en la API de Win32
//---------------------------------------------------------------

// Ficheros include:
#include <windows.h>
#include <stdio.h>

#define CM_PRUEBA 100
#define CM_SALIR  101
#define CM_AYUDA  102

#define ID_TEXTO1 201
#define ID_TEXTO2 202
#define ID_TEXTO3 203
#define ID_TEXTO4 204

#define ID_BOTON_SUMA   301
#define ID_BOTON_RESTA  302
#define ID_BOTON_MULTI  303
#define ID_BOTON_DIV    304
#define ID_BOTON_BORRAR 305
#define ID_BOTON_SALIR  306

#define ID_ESTAT1 401
#define ID_ESTAT2 402

// Prototipos:
LRESULT CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM);
void InsertarMenu(HWND);
void InitForm(HWND, WPARAM, LPARAM);
void add2History(HWND, char *);

// Definición de funciones:

//------------------------------------------------
// WinMain: función de inicio del programa
//------------------------------------------------
int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
    /* Declaración: */
    HWND hwnd;
    MSG mensaje;
    WNDCLASSEX wincl;
    /* Inicialización: */
    /* Estructura de la ventana */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = "NUESTRA_CLASE";
    wincl.lpfnWndProc = WinProc;
    wincl.style = CS_DBLCLKS;
    wincl.cbSize = sizeof(WNDCLASSEX);
    /* Usar icono y puntero por defecto */
    wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;
    wincl.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
    /* Registrar la clase de ventana, si falla, salir del programa */
    if(!RegisterClassEx(&wincl))
        return 0;

    hwnd = CreateWindowEx( 0,
                           "NUESTRA_CLASE",
                           "Ejemplo de GUI basado en la API de Win32",
                           WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                           1000, 825,  /* tamaño en píxeles */
                           HWND_DESKTOP,
                           NULL,
                           hThisInstance,
                           NULL );

    InsertarMenu(hwnd);

    ShowWindow(hwnd, SW_SHOWDEFAULT);
    /* Bucle de mensajes: */
    while(TRUE == GetMessage(&mensaje, 0, 0, 0))
    {
        TranslateMessage(&mensaje);
        DispatchMessage(&mensaje);
    }
    return mensaje.wParam;
}

//----------------------------------------------------
// Procesamiento de los mensajes
//----------------------------------------------------
LRESULT CALLBACK WinProc(
    HWND hwnd,     // Manipulador de ventana
    UINT msg,      // Mensaje
    WPARAM wParam, // Parámetro palabra, varía
    LPARAM lParam  // Parámetro doble palabra, varía
)
{
    char txt[1024], txt2[64], txt3[64];
    int n, d;

    switch (msg) /* manipulador del mensaje */
    {
    case WM_CREATE:
        InitForm(hwnd, wParam, lParam);
        break;

    case WM_DESTROY:
        PostQuitMessage(0); /* envía un mensaje WM_QUIT a la cola de mensajes */
        break;

    case WM_COMMAND:
        switch( LOWORD(wParam) )
        {
        case CM_AYUDA:
            MessageBox(hwnd, "No puedo ayudarte", "Lo lamento !!", MB_OK | MB_ICONERROR);
            break;

        case ID_BOTON_SUMA:
            n = GetDlgItemInt(hwnd, ID_TEXTO1, NULL, TRUE);
            n += GetDlgItemInt(hwnd, ID_TEXTO2, NULL, TRUE);
            SetDlgItemInt(hwnd, ID_TEXTO3, n, TRUE);
            add2History(hwnd, " + ");
            break;

        case ID_BOTON_RESTA:
            n = GetDlgItemInt(hwnd, ID_TEXTO1, NULL, TRUE);
            n -= GetDlgItemInt(hwnd, ID_TEXTO2, NULL, TRUE);
            SetDlgItemInt(hwnd, ID_TEXTO3, n, TRUE);
            add2History(hwnd, " - ");
            break;

        case ID_BOTON_MULTI:
            n = GetDlgItemInt(hwnd, ID_TEXTO1, NULL, TRUE);
            n *= GetDlgItemInt(hwnd, ID_TEXTO2, NULL, TRUE);
            SetDlgItemInt(hwnd, ID_TEXTO3, n, TRUE);
            add2History(hwnd, " X ");
            break;

        case ID_BOTON_DIV:
            n = GetDlgItemInt(hwnd, ID_TEXTO1, NULL, TRUE);
            d = GetDlgItemInt(hwnd, ID_TEXTO2, NULL, TRUE);
            if( d )
            {
                n /= d;
                SetDlgItemInt(hwnd, ID_TEXTO3, n, TRUE);
            }else{
                SetDlgItemText(hwnd, ID_TEXTO3, "Div. por cero");
                MessageBox(hwnd, "División por cero", "ERROR !!", MB_OK | MB_ICONERROR);
            }
            add2History(hwnd, " / ");
            break;

        case ID_BOTON_BORRAR:
            SetDlgItemText(hwnd, ID_TEXTO4, "");
            break;

        case CM_SALIR:
        case ID_BOTON_SALIR:
            if( MessageBox( NULL, "ATENCIÓN", "¿Desea cerrar el programa?", MB_OKCANCEL ) == IDOK )
                PostMessage(hwnd, WM_CLOSE, 0, 0);
            break;
        }

        break;
    default: /* para los mensajes de los que no nos ocupamos */
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

//-------------------------------------------------
// Inserta el menú en el formulario indicado
//-------------------------------------------------
void InsertarMenu(HWND hWnd)
{
    HMENU hMenu1, hMenu2, hMenu3;

    hMenu1 = CreateMenu();
    /* Manipulador de la barra de menú */
    hMenu2 = CreateMenu();
    hMenu3 = CreateMenu();
    /* Manipulador para el primer menú pop-up */
    AppendMenu(hMenu2, MF_STRING, CM_PRUEBA, "&Prueba");   /* 1º ítem */
    AppendMenu(hMenu2, MF_SEPARATOR, 0, NULL); /* 2º ítem (separador) */
    AppendMenu(hMenu2, MF_STRING, CM_SALIR, "&Salir"); /* 3º ítem */
    /* Inserción del menú pop-up */
    AppendMenu(hMenu1, MF_STRING | MF_POPUP, (UINT)hMenu2, "&Archivo");
    AppendMenu(hMenu1, MF_STRING, CM_AYUDA, "&Ayuda");
    SetMenu (hWnd, hMenu1); /* Asigna el menú a la ventana hWnd */
}

//-------------------------------------------------
// Crea y dimensiona los controles del formulario
//-------------------------------------------------
void InitForm(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    // Creación del formulario. Crea e inicializa los controles de la ventana.

    HWND hButtonSuma, hButtonResta, hButtonMulti, hButtonDiv, hButtonBorrar, hButtonSalir;
    HWND hEdit1, hEdit2, hEdit3, hEdit4, hEdit5, hEdit6, hLabel1, hLabel2;

    // Insertar botones
    hButtonSuma = CreateWindow("BUTTON",
                           "Atender Paciente",
                           BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP,
                           750,140,
                           150,24,
                           hwnd,
                           (HMENU)ID_BOTON_SUMA,
                           GetModuleHandle(NULL),
                           NULL);
    if(hButtonSuma == NULL)
        MessageBox(hwnd, "No se pudo crear un botón", "Error", MB_OK | MB_ICONERROR);

    hButtonResta = CreateWindow("BUTTON",
                           "Listado de pacientes por obra social",
                           BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP,
                           36,690,
                           300,24,
                           hwnd,
                           (HMENU)ID_BOTON_RESTA,
                           GetModuleHandle(NULL),
                           NULL);
    if(hButtonResta == NULL)
        MessageBox(hwnd, "No se pudo crear un botón", "Error", MB_OK | MB_ICONERROR);

    hButtonMulti = CreateWindow("BUTTON",
                           "Reiniciar",
                           BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP,
                           540,690,
                           100,24,
                           hwnd,
                           (HMENU)ID_BOTON_MULTI,
                           GetModuleHandle(NULL),
                           NULL);
    if(hButtonMulti == NULL)
        MessageBox(hwnd, "No se pudo crear un botón", "Error", MB_OK | MB_ICONERROR);

    hButtonDiv = CreateWindow("BUTTON",
                           "Recuperar el Archivo",
                           BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP,
                           650,690,
                           160,24,
                           hwnd,
                           (HMENU)ID_BOTON_DIV,
                           GetModuleHandle(NULL),
                           NULL);
    if(hButtonDiv == NULL)
        MessageBox(hwnd, "No se pudo crear un botón", "Error", MB_OK | MB_ICONERROR);

    hButtonBorrar = CreateWindow("BUTTON",
                           "Grabar Archivo",
                           BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP,
                           820,690,
                           160,24,
                           hwnd,
                           (HMENU)ID_BOTON_BORRAR,
                           GetModuleHandle(NULL),
                           NULL);
    if(hButtonBorrar == NULL)
        MessageBox(hwnd, "No se pudo crear un botón", "Error", MB_OK | MB_ICONERROR);

    hButtonSalir = CreateWindow("BUTTON",
                           "Salir",
                           BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP,
                           900,730,
                           60,24,
                           hwnd,
                           (HMENU)ID_BOTON_SALIR,
                           GetModuleHandle(NULL),
                           NULL);
    if(hButtonSalir == NULL)
        MessageBox(hwnd, "No se pudo crear un botón", "Error", MB_OK | MB_ICONERROR);

    HINSTANCE hInstance = ((LPCREATESTRUCT)lParam)->hInstance;

    // Insertar controles Edit
    hEdit1 = CreateWindowEx( 0,
                             "EDIT",  /* Nombre de la clase */
                             "",      /* Texto del título, no tiene */
                             ES_RIGHT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP, /* Estilo */
                             36, 45,  /* Posición */
                             120, 20, /* Tamaño */
                             hwnd,    /* Ventana padre */
                             (HMENU)ID_TEXTO1, /* Identificador del control */
                             hInstance, /* Instancia */
                             NULL);     /* Sin datos de creación de ventana */
    if(hEdit1 == NULL)
        MessageBox(hwnd, "No se pudo crear un Edit", "Error", MB_OK | MB_ICONERROR);

    /* Insertar control Edit */
    hEdit2 = CreateWindowEx( 0,
                             "EDIT",  /* Nombre de la clase */
                             "",      /* Texto del título, no tiene */
                             ES_RIGHT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP, /* Estilo */
                             205, 45,  /* Posición */
                             120, 20, /* Tamaño */
                             hwnd,    /* Ventana padre */
                             (HMENU)ID_TEXTO2, /* Identificador del control */
                             hInstance, /* Instancia */
                             NULL);     /* Sin datos de creación de ventana */
    if(hEdit2 == NULL)
        MessageBox(hwnd, "No se pudo crear un Edit", "Error", MB_OK | MB_ICONERROR);

    /* Insertar control Edit */
    hEdit3 = CreateWindowEx( 0,
                             "EDIT",  /* Nombre de la clase */
                             "",      /* Texto del título, no tiene */
                             ES_RIGHT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP, /* Estilo */
                             360, 45,  /* Posición */
                             120, 20,  /* Tamaño */
                             hwnd,     /* Ventana padre */
                             (HMENU)ID_TEXTO3, /* Identificador del control */
                             hInstance, /* Instancia */
                             NULL);     /* Sin datos de creación de ventana */
    if(hEdit3 == NULL)
        MessageBox(hwnd, "No se pudo crear un Edit", "Error", MB_OK | MB_ICONERROR);

    /* Insertar control Edit */
    hEdit4 = CreateWindowEx( 0,
                             "EDIT",  /* Nombre de la clase */
                             "",      /* Texto del título, no tiene */
                             ES_LEFT | ES_READONLY | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL | WS_VSCROLL | WS_HSCROLL , /* Estilo */
                             20, 175,  /* Posición */
                             285,500,  /* Tamaño */
                             hwnd,     /* Ventana padre */
                             (HMENU)ID_TEXTO4, /* Identificador del control */
                             hInstance, /* Instancia */
                             NULL);     /* Sin datos de creación de ventana */
    if(hEdit4 == NULL)
        MessageBox(hwnd, "No se pudo crear un Edit", "Error", MB_OK | MB_ICONERROR);

         hEdit5 = CreateWindowEx( 0,
                             "EDIT",  /* Nombre de la clase */
                             "",      /* Texto del título, no tiene */
                             ES_LEFT | ES_READONLY | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL | WS_VSCROLL | WS_HSCROLL , /* Estilo */
                             355, 175,  /* Posición */
                             285,500,  /* Tamaño */
                             hwnd,     /* Ventana padre */
                             (HMENU)ID_TEXTO4, /* Identificador del control */
                             hInstance, /* Instancia */
                             NULL);     /* Sin datos de creación de ventana */
    if(hEdit5 == NULL)
        MessageBox(hwnd, "No se pudo crear un Edit", "Error", MB_OK | MB_ICONERROR);

         hEdit6 = CreateWindowEx( 0,
                             "EDIT",  /* Nombre de la clase */
                             "",      /* Texto del título, no tiene */
                             ES_LEFT | ES_READONLY | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL | WS_VSCROLL | WS_HSCROLL , /* Estilo */
                             690, 175,  /* Posición */
                             285,500,  /* Tamaño */
                             hwnd,     /* Ventana padre */
                             (HMENU)ID_TEXTO4, /* Identificador del control */
                             hInstance, /* Instancia */
                             NULL);     /* Sin datos de creación de ventana */
    if(hEdit6 == NULL)
        MessageBox(hwnd, "No se pudo crear un Edit", "Error", MB_OK | MB_ICONERROR);

    // Insertar etiqueta con el signo igual
    hLabel1 = CreateWindowEx( 0,
                             "STATIC", /* Nombre de la clase */
                             "=", /* Texto del título */
                             SS_SIMPLE | WS_CHILD | WS_VISIBLE, /* Estilo */
                             342, 45, /* Posición */
                             15, 15, /* Tamaño */
                             hwnd, /* Ventana padre */
                             (HMENU)ID_ESTAT1, /* Identificador del control */
                             hInstance, /* Instancia */
                             NULL); /* Sin datos de creación de ventana */
    if(hLabel1 == NULL)
        MessageBox(hwnd, "No se pudo crear una Etiqueta", "Error", MB_OK | MB_ICONERROR);

    // Insertar etiqueta con el signo igual
    hLabel2 = CreateWindowEx( 0,
                             "STATIC",    /* Nombre de la clase */
                             "Historial", /* Texto del título */
                             SS_SIMPLE | WS_CHILD | WS_VISIBLE, /* Estilo */
                             36, 130,     /* Posición */
                             60, 45, /* Tamaño */
                             hwnd, /* Ventana padre */
                             (HMENU)ID_ESTAT2, /* Identificador del control */
                             hInstance, /* Instancia */
                             NULL); /* Sin datos de creación de ventana */
    if(hLabel2 == NULL)
        MessageBox(hwnd, "No se pudo crear una Etiqueta", "Error", MB_OK | MB_ICONERROR);

    /* Inicialización de los datos de la aplicación */
    SetDlgItemText(hwnd, ID_TEXTO1, "");
    SetDlgItemText(hwnd, ID_TEXTO2, "");
    SetDlgItemText(hwnd, ID_TEXTO3, "");

    SendMessage(GetDlgItem(hwnd, ID_TEXTO3), EM_SETREADONLY, TRUE, 0);

    SetFocus(hEdit1);
}

//-------------------------------------------------------
// Agrega la última operación al historial de cálculos
//-------------------------------------------------------
void add2History(HWND hwnd, char * op)
{
    char txt[1024], txt2[64], txt3[32];

    GetDlgItemText(hwnd, ID_TEXTO1, txt2, 64);
    GetDlgItemText(hwnd, ID_TEXTO2, txt3, 64);
    strcat(txt2, op);
    strcat(txt2, txt3);
    strcat(txt2, " = ");
    GetDlgItemText(hwnd, ID_TEXTO3, txt3, 64);
    strcat(txt2, txt3);
    GetDlgItemText(hwnd, ID_TEXTO4, txt, 1024);
    strcat(txt,txt2);
    strcat(txt,"\r\n");
    SetDlgItemText(hwnd, ID_TEXTO4, txt);
}
