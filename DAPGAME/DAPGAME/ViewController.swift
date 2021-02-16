//
//  ViewController.swift
//  Draw and Play Video Game
//
//  Created by user183570 on 11/11/20.
//

import UIKit

class ViewController: UIViewController, UIImagePickerControllerDelegate, UINavigationControllerDelegate {
    @IBAction func takePhoto(_ sender: Any) {
        let actionSheetController = UIAlertController(title: "Choose your image", message: nil, preferredStyle: .actionSheet)
        
        let cancelAction = UIAlertAction(title: "Cancel", style: UIAlertAction.Style.cancel) { (alertAction) -> Void in
            //do nothing
        }
        
        let takingPicturesAction = UIAlertAction(title: "Take Photo", style: UIAlertAction.Style.destructive) { (alertAction) -> Void in
            self.getImageGo(type: 1)
        }
        
        let photoAlbumAction = UIAlertAction(title: "Album", style: UIAlertAction.Style.default) { (alertAction) -> Void in
            self.getImageGo(type: 2)
        }
                
        actionSheetController.addAction(cancelAction)
        actionSheetController.addAction(takingPicturesAction)
        actionSheetController.addAction(photoAlbumAction)
        
        //Set anchor point for iPad device floating layer
        actionSheetController.popoverPresentationController?.sourceView = sender as? UIView
        //show
        self.present(actionSheetController, animated: true, completion: nil)
    }
    
    @IBOutlet weak var imageView: UIImageView!
    var takingPicture:UIImagePickerController!
    
    override func viewDidLoad() {	
        super.viewDidLoad()
        // Do any additional setup after loading the view.
        
//        let button = UIButton.init(type: .custom)
//
//                view.addSubview(button)
//                button.backgroundColor = UIColor.red
//                button.frame = CGRect.init(x: 100, y: 100, width: 200, height: 30)
//                button.tag = 1
//                button.addTarget(self, action: #selector(tapped), for: .touchUpInside)
    }
    
    
    func getImageGo(type:Int){
        takingPicture =  UIImagePickerController.init()
        if(type==1){
            takingPicture.sourceType = .camera
            //Whether to show the toolbar when taking pictures
            //takingPicture.showsCameraControls = true
        }else if(type==2){
            takingPicture.sourceType = .photoLibrary
        }
        //Whether to intercept, set to true to intercept the picture into a square after obtaining it
        takingPicture.allowsEditing = false
        takingPicture.delegate = self
        present(takingPicture, animated: true, completion: nil)
    }
    
    //Take a photo or choose the returned picture from the album
    func imagePickerController(_ picker: UIImagePickerController, didFinishPickingMediaWithInfo info: [UIImagePickerController.InfoKey : Any]) {
        takingPicture.dismiss(animated: true, completion: nil)
        if(takingPicture.allowsEditing == false){
            //Original image
            let resultImage = LineDetectorBridge().detectLine(in: info[UIImagePickerController.InfoKey.originalImage]as? UIImage)
            imageView.image = resultImage
        }else{
            //Screenshot; Unprocessed
            imageView.image = info[UIImagePickerController.InfoKey.editedImage] as? UIImage
        }
 
    }

    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        super.prepare(for: segue, sender: sender)
        if let vc = segue.destination as? GameViewController {
            //vc.modalPresentationStyle = .fullScreen
            vc.message = "My Message"
        }
    }
}
