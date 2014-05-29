;; This is tool for mapping binary trace to source code
;; Author: Yufei Gu

; Put the following code in your .emacs, init.el, site-load.el, or other relevant file
; (add-to-list 'load-path "path-to-tracemapping.el")
; (require 'tracemapping)

(defun search-back-current-word ()
  "search backward by current word"
  (interactive)
  (save-restriction
    (let (start end)
      (skip-chars-backward "-_A-Za-z0-9") (setq start (point))
      (skip-chars-forward "-_A-Za-z0-9") (setq end (point))
      (setq current-word  (buffer-substring start end))

      ;TODO put some strings handle here
      (set-mark start) ;mark the whole area, optional
      (goto-char start) 
      (search-backward  current-word nil t)
      )))

(global-set-key (kbd "C-q") 'search-back-current-word)


(defun open-file-line(name line)
   "open file and goto specified line"
  (switch-to-buffer-other-window name)
  (find-file name)
  (goto-line line)
  (hl-line-highlight))


(setq src-directory "")

(defun change-src-directory(arg)
  (interactive (list (read-directory-name "Select new source code directory:")) )
  (setq src-directory arg) )

(defun goto-src-line()
  (interactive)
  (setq my-str (buffer-substring (line-beginning-position) (line-end-position)))
  (setq my-list (delete "" (split-string my-str "\t")))
  (if (> (length my-list) 4)
      (progn
        (if (string= src-directory "")
            (setq src-directory (read-directory-name "Select source code directory:")))
        (open-file-line 
         (concat src-directory (nth 4 my-list)) 
         (string-to-number (nth 3 my-list)))
        (other-window 1)
        (goto-line (+ 1 (line-number-at-pos)))))
  )

(global-set-key (kbd "C-z") 'goto-src-line)

(provide 'tracemapping)
