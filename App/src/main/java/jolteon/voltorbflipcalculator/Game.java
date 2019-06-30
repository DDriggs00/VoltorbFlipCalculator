package jolteon.voltorbflipcalculator;

import android.app.AlertDialog;
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
import android.widget.Button;
import android.widget.TextView;

public class Game extends AppCompatActivity {
    private boolean finishLastTap = false;
    private long gamePtr = 0;
    private int totalPoints = 0;
    private boolean flagging = false;
//    private boolean sideways = false;

    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_game);

        FloatingActionButton switchBtn = findViewById(R.id.switchActivity);
        switchBtn.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                Intent intent = new Intent(Game.this, MainActivity.class);
                intent.addFlags(Intent.FLAG_ACTIVITY_REORDER_TO_FRONT);
                startActivity(intent);
            }
        });

        Display display = getWindowManager().getDefaultDisplay();
        Point size = new Point();
        display.getSize(size);
        int sWidth = size.x;
        int sHeight = size.y;

        for (int i = 0; i < 5; i++) {
            for(int j = 0; j < 5; j++) {
                String btnID = "cell"  + (i+1) + (j+1);
                int rID = getResources().getIdentifier(btnID, "id", getPackageName());
                Button cell = findViewById(rID);
                cell.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        if(flagging) {
                            flagCell(v);
                        }
                        else {
                            cellButton(v);
                        }
                    }
                });
                cell.setOnLongClickListener(new View.OnLongClickListener() {
                    @Override
                    public boolean onLongClick(View v) {
                        if(flagging) {
                            cellButton(v);
                        }
                        else {
                            flagCell(v);
                        }
                        return true;
                    }
                });
                cell.getLayoutParams().width=sWidth/8;
                cell.getLayoutParams().height=sWidth/8;
            }
            String textViewID = "rowTotal" + (i+1);
            int resourceID = getResources().getIdentifier(textViewID, "id", getPackageName());
            TextView tv = findViewById(resourceID);
            tv.getLayoutParams().width=sWidth/8;
            tv.getLayoutParams().height=sWidth/8;

            textViewID = "rowVoltorb" + (i+1);
            resourceID = getResources().getIdentifier(textViewID, "id", getPackageName());
            tv = findViewById(resourceID);
            tv.getLayoutParams().width=sWidth/8;
            tv.getLayoutParams().height=sWidth/8;

            textViewID = "colTotal" + (i+1);
            resourceID = getResources().getIdentifier(textViewID, "id", getPackageName());
            tv = findViewById(resourceID);
            tv.getLayoutParams().width=sWidth/8;
            tv.getLayoutParams().height=sWidth/8;

            textViewID = "colVoltorb" + (i+1);
            resourceID = getResources().getIdentifier(textViewID, "id", getPackageName());
            tv = findViewById(resourceID);
            tv.getLayoutParams().width=sWidth/8;
            tv.getLayoutParams().height=sWidth/8;
        }

        int rID = getResources().getIdentifier("flagButton", "id", getPackageName());
        Button btn = findViewById(rID);
        btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String btnName = v.getResources().getResourceName(v.getId());
                int resourceID = getResources().getIdentifier(btnName, "id", getPackageName());
                Button btn = findViewById(resourceID);

                if(flagging) {
                    btn.getBackground().setColorFilter(0xFFd4d4d4, PorterDuff.Mode.MULTIPLY);
                    btn.setText(R.string.flag);
                }
                else {
                    btn.getBackground().setColorFilter(0xFFFFCF40, PorterDuff.Mode.MULTIPLY);
                    btn.setText(R.string.stopFlag);
                }

                flagging = !flagging;
            }
        });
        btn.getLayoutParams().width=(sWidth/8) * 3;
        btn.getLayoutParams().height=sWidth/8;

        rID = getResources().getIdentifier("quitButton", "id", getPackageName());
        btn = findViewById(rID);
        btn.getLayoutParams().width=(sWidth/8) * 3;
        btn.getLayoutParams().height=sWidth/8;

        totalPoints = getSharedPreferences("PREFERENCE", MODE_PRIVATE).getInt("totalPoints", 0);
        int resourceID = getResources().getIdentifier("yourCoins", "id", getPackageName());
        TextView tv = findViewById(resourceID);
        tv.setText(String.valueOf(totalPoints));

        finishLastTap = false;
        if(gamePtr == 0) {
            gamePtr = createGamePointer();
        }
        populateBoardTotals();
        this.setTitle("Voltorb Flip Lv. " + getCurrentLevel(gamePtr));
    }

    @Override
    public void onSaveInstanceState(Bundle savedInstanceState) {
        super.onSaveInstanceState(savedInstanceState);
        // Save UI state changes to the savedInstanceState.
        // This bundle will be passed to onCreate if the process is
        // killed and restarted.
        savedInstanceState.putLong("gamePointer", gamePtr);
        // etc.
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
                deleteGamePointer(gamePtr);
                gamePtr = createGamePointer();
                updateBoard();
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

    public void updateBoard() {
        populateBoardTotals();

        int resourceID = getResources().getIdentifier("earnedCoins", "id", getPackageName());
        TextView tv = findViewById(resourceID);
        int points = getPoints(gamePtr);
        tv.setText(String.valueOf(points));

        this.setTitle("Voltorb Flip Lv. " + getCurrentLevel(gamePtr));

        finishLastTap = false;

    }

    public void populateBoardTotals() {

        for(int i = 0; i < 5; i++) {
            String textViewID = "rowTotal" + (i+1);
            int resourceID = getResources().getIdentifier(textViewID, "id", getPackageName());
            TextView tv = findViewById(resourceID);
            int text = getTotal(false,false,i,gamePtr);
            tv.setText(String.valueOf(text));

            textViewID = "rowVoltorb" + (i+1);
            resourceID = getResources().getIdentifier(textViewID, "id", getPackageName());
            tv = findViewById(resourceID);
            text = getTotal(true,false,i,gamePtr);
            tv.setText(String.valueOf(text));

            textViewID = "colTotal" + (i+1);
            resourceID = getResources().getIdentifier(textViewID, "id", getPackageName());
            tv = findViewById(resourceID);
            text = getTotal(false,true,i,gamePtr);
            tv.setText(String.valueOf(text));

            textViewID = "colVoltorb" + (i+1);
            resourceID = getResources().getIdentifier(textViewID, "id", getPackageName());
            tv = findViewById(resourceID);
            text = getTotal(true,true,i,gamePtr);
            tv.setText(String.valueOf(text));

        }
    }

    public void cellButton(View v) {

        if(gamePtr == 0) {
            gamePtr = createGamePointer();
            populateBoardTotals();
            return;
        }
        if(finishLastTap) {
            nextLevel();
            return;
        }

        String btnName = v.getResources().getResourceName(v.getId());
        final int row = btnName.charAt(btnName.length() - 2) - '0' - 1;
        final int col = btnName.charAt(btnName.length() - 1) - '0' - 1;

        if(gameIsFlipped(row, col, gamePtr)) {
            return;
        }
        if( isFlagged( row, col, gamePtr ) ) {
            unflag( row, col, gamePtr );
        }

        int resourceID = getResources().getIdentifier(btnName, "id", getPackageName());
        Button btn= findViewById(resourceID);
        int cellVal = gameFlipCell(row,col,gamePtr);
        btn.setText(String.valueOf(cellVal));


        resourceID = getResources().getIdentifier("earnedCoins", "id", getPackageName());
        TextView tv = findViewById(resourceID);
        int points = getPoints(gamePtr);
        tv.setText(String.valueOf(points));

        if(cellVal == 0) {
            triggerLoss();
        }
        else if(isWin(gamePtr)) {
            triggerWin();
        }

    }

    public void flagCell(View v) {
        if(gamePtr == 0 || finishLastTap) {
            return;
        }

        String btnName = v.getResources().getResourceName(v.getId());
        final int row = btnName.charAt(btnName.length() - 2) - '0' - 1;
        final int col = btnName.charAt(btnName.length() - 1) - '0' - 1;

        if(gameIsFlipped(row, col, gamePtr)) {
            return;
        }


        int resourceID = getResources().getIdentifier(btnName, "id", getPackageName());
        Button btn = findViewById(resourceID);
        if(isFlagged(row, col, gamePtr)) {
            unflag(row,col,gamePtr);
            btn.getBackground().setColorFilter(0xFFd4d4d4, PorterDuff.Mode.MULTIPLY);
        }
        else {
            flag(row,col,gamePtr);
            btn.getBackground().setColorFilter(0xFFFFCF40, PorterDuff.Mode.MULTIPLY);
        }

    }

    public void resetBoard() {
        Button cell;
        TextView tv;
        int resourceID;
        for(int i = 0; i < 5; i++) {
            for(int j = 0; j < 5; j++) {
                resourceID = getResources().getIdentifier("cell"  + (i+1) + (j+1), "id", getPackageName());
                cell = findViewById(resourceID);
                cell.getBackground().setColorFilter(0xFFd4d4d4, PorterDuff.Mode.MULTIPLY);
                if( isFlagged( i, j, gamePtr ) ) {
                    unflag( i, j, gamePtr );
                }
                cell.setText(" ");
            }
            resourceID = getResources().getIdentifier("rowTotal" + (i+1), "id", getPackageName());
            tv = findViewById(resourceID);
            tv.setText("5");
            resourceID = getResources().getIdentifier("rowVoltorb" + (i+1), "id", getPackageName());
            tv = findViewById(resourceID);
            tv.setText("0");
            resourceID = getResources().getIdentifier("colTotal" + (i+1), "id", getPackageName());
            tv = findViewById(resourceID);
            tv.setText("5");
            resourceID = getResources().getIdentifier("colVoltorb" + (i+1), "id", getPackageName());
            tv = findViewById(resourceID);
            tv.setText("0");
        }
    }

    public void nextLevel() {
        finishLastTap = false;
        totalPoints += getPoints(gamePtr);
        startNextLevel(gamePtr);
        resetBoard();
        populateBoardTotals();
        this.setTitle("Voltorb Flip Lv. " + getCurrentLevel(gamePtr));
    }

    public void triggerLoss() {
        Alert("Voltorb! You Lose.","Dropped to level " + (getCurrentLevel( gamePtr ) ) +"\nTouch board to continue" );
        revealBoard();
        finishLastTap = true;
    }

    public void triggerWin() {
        Alert("You Win!","Advanced to level " + (getCurrentLevel( gamePtr ) + 1) +"\nTouch board to continue" );
        revealBoard();
        finishLastTap = true;
        totalPoints += getPoints( gamePtr );
        getSharedPreferences("PREFERENCE", MODE_PRIVATE).edit().putInt("totalPoints", totalPoints).apply();
        int resourceID = getResources().getIdentifier("yourCoins", "id", getPackageName());
        TextView tv = findViewById(resourceID);
        tv.setText(String.valueOf(totalPoints));

    }

    public void revealBoard() {
        for(int i = 0;  i < 5; i++) {
            for(int j = 0; j < 5; j++) {
                String btnID = "cell"  + (i+1) + (j+1);
                int resourceID = getResources().getIdentifier(btnID, "id", getPackageName());
                Button btn= findViewById(resourceID);
                btn.setText(String.valueOf(gameGetCell(i,j,gamePtr)));
            }
        }
    }

    public void Alert(String title, String message) {
        AlertDialog.Builder builder;
        builder = new AlertDialog.Builder(this);
        builder.setTitle(title).setMessage(message)
                .setPositiveButton(android.R.string.ok, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which){}
                }).show();
    }

    public native long createGamePointer();
    public native void deleteGamePointer(long gamePtr);
    public native int gameFlipCell(int row, int col, long gamePtr);
    public native int gameGetCell(int row, int col, long gamePtr);
    public native int getTotal(boolean isVoltorb, boolean isCol, int index, long gamePtr);
    public native int getCurrentLevel(long gamePtr);
    public native boolean gameIsFlipped(int row, int col, long gamePtr);
    public native boolean isWin(long gamePtr);
    public native int getPoints(long gamePtr);
    public native void startNextLevel(long gamePtr);
    public native void flag(int row, int col, long gamePtr);
    public native void unflag(int row, int col, long gamePtr);
    public native boolean isFlagged(int row, int col, long gamePtr);
}