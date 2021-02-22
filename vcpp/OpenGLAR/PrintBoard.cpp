#include <windows.h>
#include <iostream>
#include "resource.h"

BOOL CALLBACK DialogProc( HWND, UINT, WPARAM, LPARAM );

void Printmain(void)
{
	int nResult = DialogBoxParam(NULL, MAKEINTRESOURCE(IDD_DIALOG2), NULL, (DLGPROC)DialogProc,NULL);

}

BOOL CALLBACK DialogProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	
    //DialogData *dlgdata;
	//std::ostringstream strs;
	char szText[81];
	
	switch( uMsg )
	{
		case WM_INITDIALOG :
			/*
			dlgdata=(DialogData*)lParam;
			strs << dlgdata->radius;
			SetDlgItemText ( hDlg, IDC_EDIT_RADIUS, strs.str().c_str());
			strs.str("");
			strs << dlgdata->height;
			SetDlgItemText ( hDlg, IDC_EDIT_HEIGHT, strs.str().c_str());
			SetDlgItemInt ( hDlg, IDC_EDIT_STACK, dlgdata->stack, FALSE );
			SetDlgItemInt ( hDlg, IDC_EDIT_SLICE, dlgdata->slice, FALSE );
			SetWindowPos(hDlg,HWND_TOP,300,300,0,0,SWP_NOSIZE|SWP_SHOWWINDOW);
			*/
			return (TRUE);

		case WM_COMMAND :
			switch( LOWORD( wParam ) )
			{
				case IDOK :
					/*
					BOOL bTranslated;
					data.stack = GetDlgItemInt( hDlg, IDC_EDIT_STACK,&bTranslated,FALSE);
					data.slice = GetDlgItemInt( hDlg, IDC_EDIT_SLICE,&bTranslated,FALSE);
					GetDlgItemText(hDlg,IDC_EDIT_RADIUS,szText,sizeof(szText)-1);
					data.radius = atof(szText);
					if(data.radius<=0.0) 
					{
						MessageBox(NULL,"Incorrect radius!!!","Warning!!!",MB_OK);
						return(FALSE);
					}
					GetDlgItemText(hDlg,IDC_EDIT_HEIGHT,szText,sizeof(szText)-1);
					data.height = atof(szText);
					if(data.height<=0.0) 
					{
						MessageBox(NULL,"Incorrect height!!!","Warning!!!",MB_OK);
						return(FALSE);
					}
					*/
					EndDialog(hDlg,TRUE);
					
					return (TRUE);
				case IDCANCEL : 
					EndDialog(hDlg,FALSE);
					return (TRUE);
			}
			break;
		default :
			return( FALSE );
	}

	return( TRUE );
}
