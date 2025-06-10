//Made by Victor Baptista 
//inicialização do jogo
const ROWS = 20;
const COLS = 10;

const board = Array.from({ length: ROWS }, () => Array(COLS).fill(0));

const domBoard = Array.from({ length: ROWS }, () => Array(COLS).fill(null));

let nextQueue = [];

const COLORS = {
  I: "cyan",
  J: "blue",
  L: "orange",
  O: "yellow",
  S: "green",
  T: "purple",
  Z: "red",
};

const TETROMINOS = {
    I: [[1, 1, 1, 1]],
    J: [
        [1, 0, 0],
        [1, 1, 1],
    ],
    L: [
        [0, 0, 1],
        [1, 1, 1],
    ],
    O: [
        [1, 1],
        [1, 1],
    ],
    S: [
        [0, 1, 1],
        [1, 1, 0],
    ],
    T: [
        [0, 1, 0],
        [1, 1, 1],
    ],
    Z: [
        [1, 1, 0],
        [0, 1, 1],
    ],
};

function initializeBoard() {
  const gameBoard = document.getElementById("game-board");
  board.forEach((row, rowIndex) => {
    row.forEach((_, colIndex) => {
      const cell = document.createElement("div");
      cell.classList.add("cell");
      gameBoard.appendChild(cell);
      domBoard[rowIndex][colIndex] = cell;
    });
  });
}

let tetromino = [];

function createTetromino() {
  const shapes = "IJLOSTZ";

  if (nextQueue.length === 0) {
    for (let i = 0; i < 3; i++) {
      const shape = shapes[Math.floor(Math.random() * shapes.length)];
      nextQueue.push({
        shape: shape,
        grid: TETROMINOS[shape],
        color: COLORS[shape],
      });
    }
  }

  const nextPiece = nextQueue.shift();
  tetromino = {
    currentShape: nextPiece.shape,
    grid: nextPiece.grid,
    color: nextPiece.color,
    row: 0,
    col: 3,
  };

  if (collide(tetromino.row, tetromino.col)) {
    gameOver();
    if (musicOn)mainTheme99.pause();
    return; 
  }

  const newShape = shapes[Math.floor(Math.random() * shapes.length)];
  nextQueue.push({
    shape: newShape,
    grid: TETROMINOS[newShape],
    color: COLORS[newShape],
  });

  holdUsed = false;
  updateNextDisplay(); 
}


function updateBoard() {
  board.forEach((row, y) => {
    row.forEach((color, x) => {
      const cell = domBoard[y][x];
      if (cell) cell.style.backgroundColor = color || "";
    });
  });
}

function clearTetromino() {
  tetromino.grid.forEach((row, y) => {
    row.forEach((value, x) => {
      if (value) {
        const cell = domBoard[tetromino.row + y]?.[tetromino.col + x];
        if (cell) cell.style.backgroundColor = "";
      }
    });
  });
}

function drawTetromino() {
  tetromino.grid.forEach((row, y) => {
    row.forEach((value, x) => {
      if (value) {
        const cell = domBoard[tetromino.row + y]?.[tetromino.col + x];
        if (cell) cell.style.backgroundColor = tetromino.color;
      }
    });
  });
}

function clearRows() {
  let rowsCleared = 0;

  board.forEach((row, y) => {
    if (row.every((cell) => cell !== 0)) {
      board.splice(y, 1);
      board.unshift(Array(COLS).fill(0));
      rowsCleared++;
    }

  });

  updateBoard();

  if (rowsCleared > 0) {
    const pointsPerRows = [0, 100, 200, 300, 400]; 
    score += pointsPerRows[rowsCleared] * level / 2;

    updateLevelAndScoreDisplay();
  }
  
  switch (rowsCleared){
    case 1:
      if (!single.paused && !single.ended) {
        single.currentTime = 0; 
      }
      if(SFXOn)single.play();
    break;

    case 2:
      if (!double.paused && !double.ended) {
        double.currentTime = 0;
      }
      if(SFXOn)double.play();
    break;

    case 3:
      if (!triple.paused && !triple.ended) {
        triple.currentTime = 0;
      }
      if(SFXOn)triple.play();
    break;

    case 4:
      if (!quadruple.paused && !quadruple.ended) {
        quadruple.currentTime = 0; 
      }
      if(SFXOn)quadruple.play();
    break;
  }

}

function findLowestPosition() {
  let ghostRow = tetromino.row;

  while (!collide(ghostRow + 1, tetromino.col)) {
    ghostRow++;
  }
  return ghostRow;
}

let lastGhostCells = [];

function drawGhostPiece(ghostRow) {
  
  lastGhostCells.forEach(([row, col]) => {
      const cell = domBoard[row]?.[col];
      if (cell) {
          cell.style.border = '';
          cell.style.opacity = '';
      }
  });

  const isTouching = tetromino.grid.some((row, y) =>
      row.some((value, x) =>
          value && ghostRow + y === tetromino.row + y && tetromino.col + x === tetromino.col + x
      )
  );

  if (isTouching) {
      lastGhostCells = [];
      return;
  }

  lastGhostCells = [];
  tetromino.grid.forEach((row, y) => {
      row.forEach((value, x) => {
          if (value) {
              const cell = domBoard[ghostRow + y]?.[tetromino.col + x];
              if (cell) {
                  cell.style.border = `1px dashed ${tetromino.color}`;
                  cell.style.opacity = '0.5';
                  lastGhostCells.push([ghostRow + y, tetromino.col + x]);
              }
          }
      });
  });
}


// score level hold next


let holdPiece = null; 
let holdUsed = false; 

const holdContainer = document.getElementById("hold-container");
holdContainer.style.visibility = 'hidden';

const nextContainer = document.getElementById("next-container");
nextContainer.style.visibility = 'hidden';

let level = 1;
let score = 0;
let dropInterval = 600; 

const levelScoreContainer = document.getElementById("level-score-container");
levelScoreContainer.style.visibility = 'hidden';

function updateLevelAndScoreDisplay() {

  if ( score >= 1500*level && level < 7) {
    increaseLevel();
  }

  levelScoreContainer.innerHTML = `
    <div>LV: ${level}</div>
    <div>Score: ${score}</div>
  `;
}

function increaseLevel() {
  level++;
  
  dropInterval = Math.max(150, dropInterval - level * 50);
  
  updateLevelAndScoreDisplay();
  
}

function holdTetromino() {
  
  if (holdUsed) return;

  holdContainer.style.visibility = 'visible';
  clearTetromino(); 

  if (!holdPiece) {
    
    holdPiece = {
      shape: tetromino.currentShape,
      grid: tetromino.grid,
      color: tetromino.color,
    };
    createTetromino(); 
  } else {
    const temp = {
      shape: tetromino.currentShape,
      grid: tetromino.grid,
      color: tetromino.color,
    };

    tetromino.currentShape = holdPiece.shape;
    tetromino.grid = holdPiece.grid;
    tetromino.color = holdPiece.color;
    holdPiece = temp;
    tetromino.row = 0; 
    tetromino.col = 3;
  }

  holdUsed = true; 
  drawTetromino(); 
  updateHoldDisplay(); 

  if (!hold.paused && !hold.ended) {
    hold.currentTime = 0; 
  }
  if(SFXOn) hold.play();
}

function updateHoldDisplay() {
  
  holdContainer.innerHTML = ""; 
  
  if (holdPiece) {
    holdPiece.grid.forEach((row) => {
      const rowDiv = document.createElement("div");
      rowDiv.classList.add("hold-row");
      row.forEach((cell) => {
        const cellDiv = document.createElement("div");
        cellDiv.classList.add("cell");
        if (cell) {
          cellDiv.classList.add("active");
          cellDiv.style.setProperty("--tetromino-color", holdPiece.color); 
        }
        rowDiv.appendChild(cellDiv);
      });
      holdContainer.appendChild(rowDiv);
    });
  }
}

function updateNextDisplay() {
  
  nextContainer.innerHTML = ""; 

  nextQueue.forEach((piece, index) => {
    const pieceGrid = piece.grid;

   
    const pieceDiv = document.createElement("div");
    pieceDiv.classList.add("next-piece");
    pieceDiv.style.marginBottom = "10px";

    pieceGrid.forEach((row) => {
      const rowDiv = document.createElement("div");
      rowDiv.classList.add("next-row");

      row.forEach((cell) => {
        const cellDiv = document.createElement("div");
        cellDiv.classList.add("cell");
        if (cell) {
          cellDiv.classList.add("active");
          cellDiv.style.setProperty("--tetromino-color", piece.color);
        }
        rowDiv.appendChild(cellDiv);
      });

      pieceDiv.appendChild(rowDiv);
    });

    nextContainer.appendChild(pieceDiv);
  });
}


//funções de movimento 


let lastAutoPlaceTime = 0; 

function autoPlaceTetromino() {
  const placementCooldown = 25; 
  const currentTime = Date.now();

  
  if (currentTime - lastAutoPlaceTime < placementCooldown) {
    return;
  }

  lastAutoPlaceTime = currentTime; 
  clearTetromino();
  const lowestRow = findLowestPosition();
  tetromino.row = lowestRow;

  merge();
  createTetromino();
  drawTetromino();
  score += 10*level;
  updateLevelAndScoreDisplay();

  if (!hardDrop.paused && !hardDrop.ended) {
    hardDrop.currentTime = 0; 
    
  }
  if(SFXOn)hardDrop.play();
}

function autoMoveDown() {
  clearTetromino();
  tetromino.row++;
  if (collide()) {
    tetromino.row--;
    merge();
    createTetromino();
  }
  drawTetromino();
  score += 1 * level;
  
  updateLevelAndScoreDisplay();
}

function moveLeft() {
  clearTetromino();
  tetromino.col--;
  if (collide()) tetromino.col++;
  drawTetromino();

  if (!move.paused && !move.ended) {
    move.currentTime = 0; 
  }
  if(SFXOn)move.play();
}

function moveRight() {
  clearTetromino();
  tetromino.col++;
  if (collide()) tetromino.col--;
  drawTetromino();

  if (!move.paused && !move.ended) {
    move.currentTime = 0; 
  }
  if(SFXOn)move.play();
}

let lastRotationTime = 0; 

let previousGrid; 
let previousCol;


function spining() {
  previousGrid = tetromino.grid;
  previousCol = tetromino.col;

  const rotated = tetromino.grid[0]
    .map((_, i) => tetromino.grid.map((row) => row[i]))
    .reverse();
  
  tetromino.grid = rotated;

  if (!spin.paused && !spin.ended) {
    spin.currentTime = 0; 
  }
  if(SFXOn)spin.play();
}

function rotateTetromino() {
  const rotationDelay = 100; 
  const currentTime = Date.now();

  
  if (currentTime - lastRotationTime < rotationDelay) {
    return;
  }
  lastRotationTime = currentTime;

  clearTetromino(); 

  
  if (tetromino.currentShape === "I") {
    
    if (tetromino.col === 7 || tetromino.col === 8 || tetromino.col === 9) {
      tetromino.col = 6; 
      spining();
    }
    else{
      spining();
    }
  } else if (tetromino.currentShape === "S" || tetromino.currentShape === "J" || tetromino.currentShape === "T" || tetromino.currentShape === "L" || tetromino.currentShape === "Z" ) {
    if (tetromino.col === 8) {
      tetromino.col = 7; 
      spining();
    }
    else{
      spining();
    }
  }else{
    spining();
  } 

  
  if (collide()) {
    
    tetromino.grid = previousGrid;
    tetromino.col = previousCol;
  }

  drawTetromino(); 
}

function collide(testRow = tetromino.row, testCol = tetromino.col) {
  return tetromino.grid.some((row, y) =>
    row.some(
      (value, x) =>
        value &&
        (testRow + y >= ROWS ||
          testCol + x < 0 ||
          testCol + x >= COLS ||
          board[testRow + y]?.[testCol + x])
    )
  );
}

function merge() {
  
  tetromino.grid.forEach((row, y) => {
    row.forEach((value, x) => {
      if (value) {
        board[tetromino.row + y][tetromino.col + x] = tetromino.color;
      }
    });
  });

  updateBoard();
  if (!landing.paused && !landing.ended) {
    landing.currentTime = 0; 
  }
  if(SFXOn)landing.play();
}


//Menu da pausa


let isPaused = false; 
let isGameRunning = false;
let gameLoopTimeout; 
let musicOn = false;
let SFXOn = false;


let pauseButton = document.getElementById("pause-button")
pauseButton.addEventListener("click", pauseGame);
pauseButton.style.visibility = 'hidden';

let resumeButton = document.getElementById("resume-button")
resumeButton.addEventListener("click", resumeGame);

let restartButton= document.getElementById("restart-button")
restartButton.addEventListener("click", restartGame);

let SFXButton = document.getElementById('SFX-button')
SFXButton.addEventListener('click',toogleSFX )

let musicButton = document.getElementById('music-button')
musicButton.addEventListener('click',toogleMusic )

function pauseGame() {
  if (!isGameRunning || isPaused) return; 
  isPaused = true;

  
  levelScoreContainer.style.visibility = 'hidden';
  pauseButton.style.visibility = 'hidden';
  nextContainer.style.visibility = 'hidden';
  holdContainer.style.visibility = 'hidden';

  mainTheme99.pause();
  if (!pauseSound.paused && !pauseSound.ended) {
    pauseSound.currentTime = 0; 
  }
  if (SFXOn) {
    pauseSound.play();
  }

  clearTimeout(gameLoopTimeout); 
  document.getElementById("pause-menu").classList.remove("hidden"); 
}

function resumeGame() {
  if (!isGameRunning || !isPaused) return; 

  isPaused = false;

  levelScoreContainer.style.visibility = 'visible';
  pauseButton.style.visibility = 'visible';
  nextContainer.style.visibility = 'visible';
  if (holdUsed)holdContainer.style.visibility = 'visible';

  if(musicOn)mainTheme99.play();
  mainTheme99.loop = true;
  document.getElementById("pause-menu").classList.add("hidden"); 
  gameLoop(); 
}

function restartGame() {
  holdPiece = null;
  holdUsed = false;
  isPaused = false;
  isGameRunning = true;

  gameOverYeah.pause();

  clearTimeout(gameLoopTimeout);

  document.getElementById("pause-menu").classList.add("hidden"); 
  document.getElementById("game-over").classList.add("hidden");
  
  
  board.forEach((row) => row.fill(0));
  updateBoard(); 

  nextQueue = [];
  tetromino = null; 
  createTetromino();

  level = 1;
  score = 0;
  linesCleared = 0;
  dropInterval = 500;

  holdContainer.style.visibility = 'hidden';
  
  
  mainTheme99.currentTime = 0;
  if (musicOn)mainTheme99.play();
  mainTheme99.loop = true;
  
  gameStart(); 
}

function toogleMusic(){
  if (musicOn) {
    musicButton.classList.remove('status-on');
    musicButton.classList.add('status-off');
    musicOn = false;
  } else {
    musicButton.classList.remove('status-off');
    musicButton.classList.add('status-on');
    musicOn = true;
  }
}

function toogleSFX(){
  if (SFXOn) {
    SFXButton.classList.remove('status-on');
    SFXButton.classList.add('status-off');
    SFXOn = false;
  } else {
    SFXButton.classList.remove('status-off');
    SFXButton.classList.add('status-on');
    SFXOn = true;
  }
}


//Chamar as outras funções para criar o jogo


let currentTime = 0;
const doublePressDelay = 300;


let mainTheme99 = new Audio('../tetris/audio/Tetris99Theme.mp3');
let clear = new Audio('../tetris/audio/clear.wav');
let hardDrop = new Audio('../tetris/audio/harddrop.wav');
let hold = new Audio('../tetris/audio/hold.wav');
let landing = new Audio('../tetris/audio/landing.wav');
let move = new Audio('../tetris/audio/move.wav');
let pauseSound = new Audio('../tetris/audio/pause.wav');
let spin = new Audio('../tetris/audio/spin.wav')
let single = new Audio('../tetris/audio/single.wav');
let double = new Audio('../tetris/audio/double.wav');
let triple = new Audio('../tetris/audio/triple.wav');
let quadruple = new Audio('../tetris/audio/quadruple.wav');
let gameOverYeah = new Audio('../tetris/audio/Game_Over_Yeah!.mp3');

function setupControls() {

  document.removeEventListener("keydown", keydownHandler); 
  document.addEventListener("keydown", keydownHandler); 
  
}

function keydownHandler(event) {
  if (!isGameRunning || isPaused) return;
  if (event.key === "Shift") {
    holdTetromino();
  } else if (event.key === "ArrowLeft") {
    moveLeft();
  } else if (event.key === "ArrowRight") {
    moveRight();
  } else if (event.key === "ArrowUp") {
    rotateTetromino();
  } else if (event.key === "ArrowDown") {
    autoMoveDown();
  } else if (event.code === "Space") {
    autoPlaceTetromino();
  }
  const ghostRow = findLowestPosition();
  drawGhostPiece(ghostRow);
}

function gameLoop() {
  if (!isGameRunning || isPaused) return; 
  
  autoMoveDown();
  clearRows();

  const ghostRow = findLowestPosition();
  drawGhostPiece(ghostRow);

  drawTetromino();
  updateHoldDisplay();

  gameLoopTimeout = setTimeout(gameLoop, dropInterval); 

  console.log(dropInterval);
}

function gameOver() {
  
  isGameRunning = false;
  gameOverYeah.currentTime = 0;

  holdContainer.style.visibility = 'hidden';
  nextContainer.style.visibility = 'hidden';
  pauseButton.style.visibility = 'hidden';
  levelScoreContainer.style.visibility = 'hidden';

  const gameOverScreen = document.getElementById('game-over');
  const finalLevel = document.getElementById('final-level');
  const finalScore = document.getElementById('final-score');

  
  finalLevel.textContent = level; 
  finalScore.textContent = score; 

  gameOverScreen.classList.remove('hidden');

  
  const gameOverRestart = document.getElementById('gameover-restart');
  gameOverRestart.classList.remove('hidden');

  
  gameOverRestart.addEventListener("click", restartGame);

  if (!gameOverYeah.paused && !gameOverYeah.ended) {
    gameOverYeah.currentTime = 0; 
  }
  if(SFXOn){
    gameOverYeah.play();
  }
}

function gameStart() {
  
  levelScoreContainer.style.visibility = 'visible';
  pauseButton.style.visibility = 'visible';
  nextContainer.style.visibility = 'visible';
  
  setupControls();
  updateLevelAndScoreDisplay();
  createTetromino();

  isGameRunning = true; 
  gameLoop(); 
}

document.addEventListener('DOMContentLoaded', () => {
  const playButton = document.getElementById('play-button');
  const overlay = document.getElementById('overlay');


  playButton.addEventListener('click', () => {

      musicOn = true;
      SFXOn = true;
      musicButton.classList.add('status-on');
      SFXButton.classList.add('status-on');
      if (musicOn)mainTheme99.play();
      mainTheme99.loop = true;
      
      overlay.style.display = 'none';
      initializeBoard();
      gameStart(); 
  });
});
