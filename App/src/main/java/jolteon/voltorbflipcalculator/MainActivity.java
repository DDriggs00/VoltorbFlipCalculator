package jolteon.voltorbflipcalculator;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.graphics.Point;
import android.graphics.PorterDuff;
import android.support.design.widget.FloatingActionButton;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.Display;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.inputmethod.InputMethodManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.Toast;

import java.util.Objects;

public class MainActivity extends AppCompatActivity {

    private long cppClassPtr = 0;
    private boolean sideways = false;
    private String[][] PreviousValue = new String[5][5];

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Button calcButton = findViewById(R.id.CalculateButton);
        calcButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                calculateButton(v);
            }
        });

        FloatingActionButton switchBtn = findViewById(R.id.switchActivity);
        switchBtn.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                Intent intent = new Intent(MainActivity.this, Game.class);
                intent.addFlags(Intent.FLAG_ACTIVITY_REORDER_TO_FRONT);
                startActivity(intent);
            }
        });

        Display display = getWindowManager().getDefaultDisplay();
        Point size = new Point();
        display.getSize(size);
        int sWidth = size.x;
        int sHeight = size.y;

        calcButton.getLayoutParams().width = sWidth/4;
        calcButton.getLayoutParams().height = sWidth/4;

        for (int i = 0; i < 5; i++) {
            for(int j = 0; j < 5; j++) {
                PreviousValue[i][j] = "1";
                String btnID = "cell"  + (i+1) + (j+1);
                int rID = getResources().getIdentifier(btnID, "id", getPackageName());
                Button cell = findViewById(rID);
                cell.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        cellButton(v);
                    }
                });
                cell.getLayoutParams().width=sWidth/8;
                cell.getLayoutParams().height=sWidth/8;
            }
            String editTextID = "rowTotal" + (i+1);
            int resourceID = getResources().getIdentifier(editTextID, "id", getPackageName());
            EditText et = findViewById(resourceID);
            et.getLayoutParams().width=sWidth/8;
            et.getLayoutParams().height=sWidth/8;

            editTextID = "rowVoltorb" + (i+1);
            resourceID = getResources().getIdentifier(editTextID, "id", getPackageName());
            et = findViewById(resourceID);
            et.getLayoutParams().width=sWidth/8;
            et.getLayoutParams().height=sWidth/8;

            editTextID = "colTotal" + (i+1);
            resourceID = getResources().getIdentifier(editTextID, "id", getPackageName());
            et = findViewById(resourceID);
            et.getLayoutParams().width=sWidth/8;
            et.getLayoutParams().height=sWidth/8;

            editTextID = "colVoltorb" + (i+1);
            resourceID = getResources().getIdentifier(editTextID, "id", getPackageName());
            et = findViewById(resourceID);
            et.getLayoutParams().width=sWidth/8;
            et.getLayoutParams().height=sWidth/8;
        }

        FirstRunInfo();
    }

    @Override
    public boolean onPrepareOptionsMenu(Menu menu) {
        menu.clear();
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.tool_bar, menu);
        return super.onPrepareOptionsMenu(menu);

    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case R.id.help:
                LayoutInflater inflater = getLayoutInflater();
                View dialoglayout = inflater.inflate(R.layout.help_dialog, null);
                AlertDialog.Builder builder = new AlertDialog.Builder(this);
                builder.setView(dialoglayout);
                builder.show();
                break;
            case R.id.reset:
                resetBoard();
                break;
            case R.id.info:
                LayoutInflater inflater2 = getLayoutInflater();
                View dialoglayout2 = inflater2.inflate(R.layout.info_dialog, null);
                AlertDialog.Builder builder2 = new AlertDialog.Builder(this);
                builder2.setView(dialoglayout2);
                builder2.show();
        }
        return true;
    }

    public void calculateButton(View v) {

        // Hide Keyboard
        InputMethodManager imm = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
        if(imm != null) {
            imm.hideSoftInputFromWindow(Objects.requireNonNull(getCurrentFocus()).getWindowToken(), 0);
        }

        //Defocus
        LinearLayout myLayout = findViewById(R.id.defocuser);
        myLayout.requestFocus();

        //Create Input Arrays
        int[] rowTotals = new int[5];
        int[] rowVoltorbs = new int[5];
        int[] colTotals = new int[5];
        int[] colVoltorbs = new int[5];

        // Populate Input Arrays
        EditText et;
        String inputID;
        int resID;
        for (int i = 1; i <= 5; i++) {
            inputID = "rowTotal" + i;
            resID = getResources().getIdentifier(inputID, "id", getPackageName());
            et = findViewById(resID);
            if(et.getText().toString().matches("")) {
                Alert();
                return;
            }
            rowTotals[i-1] = Integer.parseInt(et.getText().toString());

            inputID = "rowVoltorb" + i;
            resID = getResources().getIdentifier(inputID, "id", getPackageName());
            et = findViewById(resID);
            if(et.getText().toString().matches("")) {
                Alert();
                return;
            }
            rowVoltorbs[i-1] = Integer.parseInt(et.getText().toString());

            inputID = "colTotal" + i;
            resID = getResources().getIdentifier(inputID, "id", getPackageName());
            et = findViewById(resID);
            if(et.getText().toString().matches("")) {
                Alert();
                return;
            }
            colTotals[i-1] = Integer.parseInt(et.getText().toString());

            inputID = "colVoltorb" + i;
            resID = getResources().getIdentifier(inputID, "id", getPackageName());
            et = findViewById(resID);
            if(et.getText().toString().matches("")) {
                Alert();
                return;
            }
            colVoltorbs[i-1] = Integer.parseInt(et.getText().toString());
        }

        // Create cpp class
        if(cppClassPtr == 0) {
            cppClassPtr = CreateCppClass();
        }
        else {
            DeleteCppClass(cppClassPtr);
            cppClassPtr = CreateCppClass();
        }

        // Verify Inputs
        if(!VerifyInputs(rowTotals, rowVoltorbs, colTotals, colVoltorbs, cppClassPtr)) {
            Alert();
            DeleteCppClass(cppClassPtr);
            cppClassPtr = 0;
        }
        else {
            if(InitialCalculations(rowTotals, rowVoltorbs, colTotals, colVoltorbs, cppClassPtr)) {

                updateBoard();
            }
            else {
                Alert();
                DeleteCppClass(cppClassPtr);
                cppClassPtr = 0;
            }

        }


    }

    public void cellButton(View v) {

        // Hide Keyboard
        InputMethodManager imm = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
        if(imm != null) {
            imm.hideSoftInputFromWindow(Objects.requireNonNull(getCurrentFocus()).getWindowToken(), 0);
        }

        //Defocus
        LinearLayout myLayout = findViewById(R.id.defocuser);
        myLayout.requestFocus();

        if(cppClassPtr == 0) {
            return;
        }

        String btnName = v.getResources().getResourceName(v.getId());
        final int row = btnName.charAt(btnName.length() - 2) - '0' - 1;
        final int col = btnName.charAt(btnName.length() - 1) - '0' - 1;


        if(IsKnown(row, col, cppClassPtr)) {
            return;
        }

        // setup the alert builder
        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        builder.setTitle("Choose a value for cell " + (row+1) + ":" + (col+1));

        String[] values = {"Voltorb", "1", "2", "3", "Cancel"};
        final boolean[] areOptions = {true, true, true, true, true};
        int totalLayouts = GetTotalLayouts(cppClassPtr);
        for(int i = 0; i < 4; i++) {
            int matchingLayouts = GetNumValues(row, col, i, cppClassPtr);
            if(matchingLayouts == 0) {
                areOptions[i] = false;
            }
            int percentMatching = (int)((double)matchingLayouts/(double)totalLayouts * 100);
            values[i] += " - [" + matchingLayouts + "] - " + percentMatching + "%";
        }
        builder.setItems(values, new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {

                if(areOptions[which]) {
                    switch (which) {
                        case 0: // V
                            resetBoard();
                            DeleteCppClass(cppClassPtr);
                            cppClassPtr = 0;
                            break;
                        case 1: // 1
                            FlipCell(row, col, 1, cppClassPtr);
                            updateBoard();
                            break;
                        case 2: // 2
                            FlipCell(row, col, 2, cppClassPtr);
                            updateBoard();
                            break;
                        case 3: // 3
                            FlipCell(row, col, 3, cppClassPtr);
                            updateBoard();
                            break;
                        case 4: // Cancel
                            break;
                        default:
                            break;
                    }
                }
                else {
                    Toast.makeText(MainActivity.this, "Sorry, That option does not exist for that cell.", Toast.LENGTH_SHORT).show();
                }
            }
        });

        // create and show the alert dialog
        AlertDialog dialog = builder.create();
        dialog.show();
    }

    public void updateBoard() {
        String outputID, CellVal;
        Button cell;
        int resourceID;
        for(int i = 0; i < 5; i++) {
            for(int j = 0; j < 5; j++) {
                outputID = "cell"  + (i+1) + (j+1);
                resourceID = getResources().getIdentifier(outputID, "id", getPackageName());
                cell = findViewById(resourceID);
                CellVal = GetCellValue(i, j, cppClassPtr).trim();

                if(IsBestCell(i, j, cppClassPtr)) {
                    cell.getBackground().setColorFilter(0xFF0079BF, PorterDuff.Mode.MULTIPLY);
                }
                else if((!CellVal.equals(PreviousValue[i][j])) && (CellVal.equals("2") || CellVal.equals("3"))) {
                    cell.getBackground().setColorFilter(0xFF00FF00, PorterDuff.Mode.MULTIPLY);
                    // SetCellColor(cell, CellVal);
                }
                else {
                    SetCellColor(cell, CellVal);
                }

                cell.setText(CellVal);
                PreviousValue[i][j] = CellVal;
            }
        }
    }

    public void resetBoard() {
        Button cell;
        EditText et;
        int resourceID;
        for(int i = 0; i < 5; i++) {
            for(int j = 0; j < 5; j++) {
                resourceID = getResources().getIdentifier("cell"  + (i+1) + (j+1), "id", getPackageName());
                cell = findViewById(resourceID);
                cell.getBackground().setColorFilter(0xFFd4d4d4, PorterDuff.Mode.MULTIPLY);
                cell.setText("1");
            }
            resourceID = getResources().getIdentifier("rowTotal" + (i+1), "id", getPackageName());
            et = findViewById(resourceID);
            et.setText("5");
            resourceID = getResources().getIdentifier("rowVoltorb" + (i+1), "id", getPackageName());
            et = findViewById(resourceID);
            et.setText("0");
            resourceID = getResources().getIdentifier("colTotal" + (i+1), "id", getPackageName());
            et = findViewById(resourceID);
            et.setText("5");
            resourceID = getResources().getIdentifier("colVoltorb" + (i+1), "id", getPackageName());
            et = findViewById(resourceID);
            et.setText("0");

        }
    }

    public void SetCellColor(Button cell, String cellVal) {
        switch (cellVal) {
            case "2":
            case "3":
            case "0%":
            case "2/3":
                cell.getBackground().setColorFilter(0xFF00AE00, PorterDuff.Mode.MULTIPLY);
                break;
            case "V":
            case "V/1":
                cell.getBackground().setColorFilter(0xFFCE0000, PorterDuff.Mode.MULTIPLY);
                break;
            case "1":
                cell.getBackground().setColorFilter(0xFFd4d4d4, PorterDuff.Mode.MULTIPLY);
                break;
            default:
                cell.getBackground().setColorFilter(0xFFFFCF40, PorterDuff.Mode.MULTIPLY);
                break;
        }
    }

    public void Alert() {
        AlertDialog.Builder builder;
        builder = new AlertDialog.Builder(this);
        builder.setTitle("Invalid Input").setMessage("Please recheck your input values.")
                .setPositiveButton(android.R.string.yes, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which){}
                }).show();
    }

    public void FirstRunInfo() {
        boolean isFirstRun = getSharedPreferences("PREFERENCE", MODE_PRIVATE).getBoolean("isFirstRun", true);
        if(isFirstRun) {
            LayoutInflater inflater2 = getLayoutInflater();
            View dialoglayout2 = inflater2.inflate(R.layout.info_dialog, null);
            AlertDialog.Builder builder2 = new AlertDialog.Builder(this);
            builder2.setView(dialoglayout2);
            builder2.show();
            getSharedPreferences("PREFERENCE", MODE_PRIVATE).edit().putBoolean("isFirstRun", false).apply();
        }
    }

    public native long CreateCppClass();
    public native void DeleteCppClass(long cppClassPtr);
    public native boolean VerifyInputs(int[] rowTotals, int[] rowVoltorbs, int[] colTotals, int[] colVoltorbs, long cppClassPtr);
    public native boolean InitialCalculations(int[] rowTotals, int[] rowVoltorbs, int[] colTotals, int[] colVoltorbs, long cppClassPtr);
    public native String GetCellValue(int row, int col, long cppClassPtr);
    public native boolean FlipCell(int row, int col, int val, long cppClassPtr);
    public native int GetNumValues(int row, int col, int val, long cppClassPtr);
    public native int GetTotalLayouts(long cppClassPtr);
    public native boolean IsBestCell(int row, int col, long cppClassPtr);
    public native boolean IsKnown(int row, int col, long cppClassPtr);
}